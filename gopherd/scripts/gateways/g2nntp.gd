#!/usr/local/bin/perl --	# -*-Perl-*-

#----------------------------------------------------------------------
# variables you might have to change:

$nntp_server = "news.tc.umn.edu";
$nntp_port   = 119;

$nntp_defart = "article";

$nntp_groups = "$0";  ### Default newsgroups file is in the same dir as script
$nntp_groups =~ s,[^/]+$,NEWSGROUPS,;

$nntp_reverse = 0;  # to list articles in reverse order

@nntp_acl=(
#     ipaddress  group    access + = allow, - = deny    
#     '.*         ^clari   -',
     '.*         .*       +'
  );
#----------------------------------------------------------------------
# end of variables

# Commands this server responds to:
# 
# ""                         -> list top level groups
# ls  [$group] [$range]      -> list group's articles and sub-groups
# lsa [$group] [$range]      -> like "", sends "article" & "lsa" commands back
# lsb [$group] [$range]      -> "" sends "body" & "lsb" commands back
# lsh [$group] [$range]      -> "" sends "header" & "lsh" commands back
# article <id>               -> get article by ID
# article $group $number     -> get 1 article, number can be "last" or "first",
#                               or any numeric perl expression.
# body <id>                  -> like article, but just get text
# body $group $number        -> ""
# head <id>                  -> like article, but just get header
# head $group $number        -> ""
# sorry                      -> send sorry message
#
# The optional $range arguement has the following format:
#      n         - return 'n' most recent articles
#     x:y        - return articles x through y (inclusive)
#                  x and y can both be numeric perl expressions. The
#                  string 'last' is replaced with the last article in the
#                  group, and the string 'first' is replaced with the first.
#   first:last   - all articles (same as not specifiying a range
#   last:first   - all articles (in reverse order)
#  last-9:last   - return last 10 articles (same as specifiying 10)
#  last:last-9   - return last 10 articles in reverse order

$Gscript = $ENV{'SCRIPT_NAME'};
$Gscript = "${Gscript}.gd" if (! ($Gscript =~ /.gd$/));
$Ghost   = $ENV{'SERVER_NAME'};
$Gport   = $ENV{'SERVER_PORT'};
$gplus   = 1 if ($0 =~ /.gpd/);

$_ = join(' ', @ARGV);

&create_groups if /^buildgroups/;

&do_find($1, $nntp_defart, $Gsearch, "find") if /^find\s+(\S+)/i;

&do_ls("",$nntp_defart,"ls") if /^$/;
&do_ls("",$nntp_defart,"ls") if /^ls\s*$/;
&do_ls($1,$nntp_defart,"ls") if /^ls\s+(.*)/i;

&do_ls("","article","lsa") if /^lsa\s*$/;
&do_ls($1,"article","lsa") if /^lsa\s+(.*)/i;

&do_ls("","body","lsb") if /^lsb\s*$/;
&do_ls($1,"body","lsb") if /^lsb\s+(.*)/i;

&do_ls("","head","lsh") if /^lsh\s*$/;
&do_ls($1,"head","lsh") if /^lsh\s+(.*)/i;

&do_article($1,$2,"ARTICLE") if /^article\s+(\S+)\s+(\S+)/i;
&do_article_id($1,"ARTICLE") if /^article\s+(<.*>)/i;

&do_article($1,$2,"HEAD") if /^head\s+(\S+)\s+(\S+)/i;
&do_article_id($1,"HEAD") if /^head\s+(<.*>)/i;

&do_article($1,$2,"BODY") if /^body\s+(\S+)\s+(\S+)/i;
&do_article_id($1,"BODY") if /^body\s+(<.*>)/i;

&Gsorry if /^sorry$/;
&Gabort("Unknown command!");
exit;

sub do_article_id {
  local($id,$cmd) = @_;
  &open_nntp;
  &Gsend("$cmd $id");
  $_ = &Grecv;
  &Gabort($_) if !/^2/;
 
  while(<GSERVER>) {
    print;
    last if /^\.\r\n$/;
  }

  &close_nntp;
  return;

}

sub do_article {
  local($group,$number,$cmd) = @_;

  if (&check_access($group) eq '-')  { &Gsorry; }
  &open_nntp;

  &Gsend("GROUP $group");
  $_ = &Grecv;
  &Gabort($_) if !/^2/;
  ($n,$f,$l) = /\d+\s+(\d+)\s+(\d+)\s+(\d+)/;

  $number = int($f) if ($number =~ m/first/);
  $number = int($l) if ($number =~ m/last/);
  $number = int($number) if (! $number);

  &Gsend("$cmd $number");
  $_ = &Grecv;
  &Gabort($_) if !/^2/;
 
  while(<GSERVER>) {
    last if /^\.\r\n$/;
    print;
  }

  &close_nntp;
  exit(0);
}

sub list_group {
  local($group,$type,$range) = @_;

  &Gsend("GROUP $group");
  $_ = &Grecv;
  &Gabort($_) if !/^211/;

  ($n,$f,$l) = /211\s+(\d+)\s+(\d+)\s+(\d+)/;

  if ($range =~ /^(\S+):(\S+)$/) {
      $low = $1; $high = $2;
      
      $low = int($low);
      $low = int($f) if (/first/);
      $low = int($l) if (/last/);

      $high = int($high);
      $high = int($f) if ($high =~ /first/);
      $high = int($l) if ($high =~ /last/);

      if ($low > $l) { $low = $l; }
      elsif ($low < $f) {$low = $f; }
      if ($high > $l) { $high = $l; }
      elsif ($high < $f) {$high = $f; }

      if ($high < $low)  {  $f = $high; $l = $low;  $nntp_reverse=1; }
      if ($low <= $high) {  $f = $low; $l = $high;  $nntp_reverse=0; }
        
  }
  elsif ($range ne '') {
       $range = int($range);
       $range = int($f) if ($range =~ /first/);
       $range = int($l) if ($range =~ /last/);
       if ($range >0 && $range < $n) {  $f = $l - $range + 1; }
  }

  &Gsend("XOVER $f-$l");
  $_ = &Grecv;
  &Gabort($_) if !/^224/;
 
  while(<GSERVER>) {
    chop; chop;

    last if /^\.$/;
    ($article,$subject, $from, $Date, $newsid, $huh, $lines, $xref) = split(/\t/, $_);
    $lines =~ s/\((.*)\)/$1/;
    $from =~ s/<.*>\s(.*)/$1/;
    $from =~ s/(.*)\s<.*>/$1/;
    $from =~ s/.*\((.*)\).*/$1/;

    $subject = sprintf("%-38.38s [%4.4s] %-20.20s", $subject, $lines, $from);

  
    $reply = "0$subject\texec:$atype $group $marticle[$i]:$Gscript\t$Ghost\t$Gport";
    if ($nntp_reverse) {
	push(@reply, $reply);
    } else {
	&GreplyDir($reply);
    }
  }

  if ($nntp_reverse) {
    for ($i=$#reply; $i!= -1; $i--) { &GreplyDir($reply[$i]); } 
  }

  &close_nntp;
}


sub find_group {
  local($group, $atype, $query, $range) = @_;

  &Gsend("GROUP $group");
  $_ = &Grecv;
  &Gabort($_) if !/^211/;

  ($n,$f,$l) = /211\s+(\d+)\s+(\d+)\s+(\d+)/;

  if ($range =~ /^(\S+):(\S+)$/) {
      $low = $1; $high = $2;
      
      $low = int($low);
      $low = int($f) if (/first/);
      $low = int($l) if (/last/);

      $high = int($high);
      $high = int($f) if ($high =~ /first/);
      $high = int($l) if ($high =~ /last/);

      if ($low > $l) { $low = $l; }
      elsif ($low < $f) {$low = $f; }
      if ($high > $l) { $high = $l; }
      elsif ($high < $f) {$high = $f; }

      if ($high < $low)  {  $f = $high; $l = $low;  $nntp_reverse=1; }
      if ($low <= $high) {  $f = $low; $l = $high;  $nntp_reverse=0; }
        
  }
  elsif ($range ne '') {
      $range = int($range);
      $range = $f if ($range =~ /first/);
      $range = $l if ($range =~ /last/);
      if ($range >0 && $range < $n) {  $f = $l - $range + 1; }
  }

  &Gsend("XHDR Subject $f-$l");
  $_ = &Grecv;
  &Gabort($_) if !/^221/;
 
  while(<GSERVER>) {
    chop; chop;
    last if /^\.$/;
    ($article,$subject) = /^(\d+)\s+(.*)/;
    $subject =~ s/\t/ /g; # just in case!
    push(@martin_subject, $subject);
    push(@marticle, $article);
  }

  &Gsend("XHDR Lines $f-$l");
  $_ = &Grecv;
  &Gabort($_) if !/^221/;

  $i=0;
  while(<GSERVER>) {
    chop; chop;
    last if /^\.$/;
    ($lines) = /^\d+\s+(.*)/;
    $lines =~ s/\t/ /g; # just in case!
    $lines =~ s/\((.*)\)/$1/;
    $martin_subject[$i] = 
      sprintf("%-38.38s [%4.4s]", $martin_subject[$i], $lines);
    $i++;
  }

  &Gsend("XHDR From $f-$l");
  $_ = &Grecv;
  &Gabort($_) if !/^221/;

  $i=0;
  while(<GSERVER>) {
    chop; chop;
    last if /^\.$/;
    ($from) = /^\d+\s+(.*)/;
    $from =~ s/\t/ /g; # just in case!
    $from =~ s/<.*>\s(.*)/$1/;
    $from =~ s/(.*)\s<.*>/$1/;
    $from =~ s/.*\((.*)\).*/$1/;

    $martin_subject[$i] = sprintf("%s %-20.20s", $martin_subject[$i], $from);

    push(@reply, "0$martin_subject[$i]\t$Ggw $atype $group $marticle[$i]\t$Ghost\t$Gport");

    $i++;
  }

  @quer=split(" ",$query);

  foreach $rep (@reply) {
      foreach $que (@quer) {
	  if(grep(/$que/i,$rep)) {
	      push(@real_reply,$rep);
	      last;
	  }
      }
  }

  if ($nntp_reverse) {
    for ($i=$#real_reply; $i!= -1; $i--) { &Greply($real_reply[$i]); } 
  }
  else {
    for ($i=$1; $i <= $#real_reply; $i++) { &Greply($real_reply[$i]); }
  }

  &close_nntp;
  &exit(0);
}

sub do_ls {
  local($prefix,$atype,$lscmd) = @_;
  local($range);

  $prefix =~ s/\s+$//;

  if ($prefix =~ /(\S+)\s+(\S+)/) {
      $prefix = $1;
      $range  = $2;
  }
  elsif (($prefix =~ /^\d+$/) || ($prefix =~ /:/)) {
      $range = $prefix;
      $prefix = "";
  }

  if (&check_access($prefix) eq '-') {
      &GreplyDir("0Sorry! No access off of campus!\texec:sorry:$Gscript\t$Ghost\t$Gport");
      exit(0);
  }

  &open_nntp;
  if ( $lscmd ne "lsa" || $prefix eq '') {
  &get_groups($prefix);
  } else {
	@groups=($prefix);
  }

  foreach(sort @groups) {
      $zin = index($_,$prefix);
      $zot = substr($_,length($prefix),1);
      $zit = substr($_,length($prefix) + 1, length($_));
      $zut = index($zit,'.');

      if($zut > 0) {
	  $zzz = substr($zit,0,$zut);
	  $zit = $zzz;
      }

      $old_thing = $_;

      if ($_ eq $prefix) { $do_list_group = $_; }
      elsif ($zin == 0 && $zot eq '.') {
	  $leaf=$zit;

	  $ov = $novs{"$prefix.$leaf"};
	  if($ov =~ /^[\t\s]/) {
	      $title = $leaf;
	  }
	  else {
	      $title = sprintf("%-15.15s $ov", $leaf);
	  }

	  $save{"$prefix.$leaf"} = 
	      "1$title\texec:$lscmd $prefix.$leaf $range:$Gscript\t$Ghost\t$Gport";

	  if (join('.',$prefix,$leaf) eq $old_thing) {
	      $save{"$prefix.$leaf (idx)"} =
		  "7$title\texec:find $prefix.$leaf $range:$Gscript\t$Ghost\t$Gport";
	  }
      }
      elsif ($prefix eq '' && /([^.]*)/) {
	  $ov = $novs{$1};
	  if($ov =~ /^[\t\s]/) {
	      $title = $1;
	  }
	  else {
	      $title = sprintf("%-15.15s $ov", $1);
	  }

	  $save{"$1"} = "1$title\texec:$lscmd $1 $range:$Gscript\t$Ghost\t$Gport";

	  if ($1 eq $old_thing) {
	      $save{"$1 (idx)"} = 
		  "7$title\texec:$lscmd $1 $range:$Gscript\t$Ghost\t$Gport";
	  }			
      }
  }

  foreach (sort keys %save) { &GreplyDir($save{$_}); }
  &list_group($do_list_group,$atype,$range) if ($do_list_group);

  &close_nntp;
  exit(0);
}

sub do_find {
  local($prefix,$atype,$query,$lscmd) = @_;
  local($range);

  $prefix =~ s/\s+$//;

  if ($prefix =~ /(\S+)\s+(\S+)/) {
      $prefix = $1;
      $range  = $2;
  }
  elsif (($prefix =~ /^\d+$/) || ($prefix =~ /:/)) {
      $range = $prefix;
      $prefix = "";
  }

  if (&check_access($prefix) eq '-') {
      &GreplyDir("0Sorry! You don't have permission for that!\t$Ggw sorry\t$Ghost\t$Gport");
      exit(0);
  }

  &open_nntp;
  &find_group($prefix,$atype,$query,$range);

  &close_nntp;
  exit(0);
}

sub open_nntp {
  local($_);
  &GopenServer($nntp_server,$nntp_port);
  $_ = &Grecv;
  &Gabort($_) if !/^2/;
}

sub close_nntp {
  &Gsend("QUIT");
  close(GSERVER);
}

#
# Get a list of groups that start with a particular prefix
#

sub get_groups {
    local($prefix)=@_;
    if (open(GROUPS,$nntp_groups)) {
	if ($prefix ne '') {
	    while(<GROUPS>) {
		chop;
		if(index($_,$prefix) == 0) {
		    ($grp,$nov) = split(/[ \t]+/, $_, 2);
		    push(@groups,$grp);
		    $novs{$grp}=$nov;
		}
	    }
	} else {
	    while(<GROUPS>) {
		chop;
		($grp,$nov) = split(/[ \t]+/, $_, 2);
		push(@groups,$grp);
		$novs{$grp}=$nov;
	    }
	}
	close(GROUPS);
    } else {                  # can't open file, get list from server!
	&load_groups;
    }
}


#
# Get newsgroups from the news server
#

sub load_groups {
  &open_nntp;
  &Gsend("LIST ACTIVE");
  $_ = &Grecv;
  &Gabort($_) if !/^215/;
  
  while(<GSERVER>) {
      chop; chop;
      last if /^\.$/;
      ($grp,$nov) = split(/[ \t]+/, $_, 2);
      if ((&check_access($grp) eq '+') || ($GnotServer == 1)) {
	  push(@groups,$grp);
      }
  }
  print "Getting Newsgroup Descriptions\n" if ($Gnotserver);
  
  &Gsend("LIST NEWSGROUPS");
  $_ = &Grecv;
  &Gabort($_) if !/^215/;

  while (<GSERVER>) {
      chop; chop;
      last if /^\.$/;
      ($grp,$nov) = split(/[ \t]+/,$_,2);
      $novs{$grp}=$nov;
  }

}

sub create_groups { 
    print "Updating NEWSGROUPS file $nntp_groups\n";
    $GnotServer = 1;
    &load_groups;
    open(GROUPS,">$nntp_groups") || die "$nntp_groups: $!";
    foreach (@groups) { 
	print GROUPS "$_ $novs{$_}\n";
    }
    close GROUPS;
    &close_nntp;
    exit(0);
}


sub check_access {
   local($group)=@_;

   return 1 if (-t STDIN);
   $ipaddress = $ENV{'REMOTE_ADDR'};

   foreach (@nntp_acl) {
      ($ipacl,$groupacl,$access)=split;
      return $access if  ($ipaddress =~ /$ipacl/) && ($group =~ /$groupacl/);
   }
   return '-'; #default is to restrict access
}




sub GopenServer {

    local($server,$port) = @_;
    local($type, $name, $aliases, $len, $saddr);

    $sockaddr = 'S n a4 x8';
    (($name, $aliases, $type, $len, $saddr) = gethostbyname($server))
	|| &Gabort("3Can't get address of: $server");
    $sin = pack($sockaddr, 2, $port, $saddr);
    socket(GSERVER, 2, 1, 0) || &Gabort("Can't create socket: $!");
    connect(GSERVER, $sin)   || &Gabort("Can't connect to server: $!");
    select(GSERVER); $| = 1; select(STDOUT); $| = 1;
}

sub GcloseServer {
    close(GSERVER);
}

sub Gsend { 
    print "send -> |$_[0]|\n" if (defined($Gdebug));
    print GSERVER "$_[0]\r\n"; 
}

sub Grecv { 
    local ($_); 
    $_= <GSERVER>; 
    s/\n$//;
    s/\r$//;
    print "recv -> |$_|\n" if (defined($Gdebug));
    return $_; 
}

sub Gabort {
    local ($mesg) = @_;
    print "0$mesg\t\terror.host\t70\r\n";
    exit;
}

sub GreplyDir {
    print "+INFO: " if ($gplus);
    print "$_[0]\n";
}

sub Gsorry {
    print "We do not allow off campus access...  Sorry\n";
    exit;
}





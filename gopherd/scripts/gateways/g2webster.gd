#!/usr/local/bin/perl

#----------------------------------------------------------------------
# variables you can change:

#$webster_server  = "webster.lcs.mit.edu";
#$webster_port    = 103;

$webster_server = "nakamichi.itlabs.umn.edu";
$webster_port   = 2627;

@webster_acl=(
#     ipaddress  access + = allow, - = deny    
     '^134.84\.      +',
     '^128.101\.     +',
     '.*         -'
  );

#----------------------------------------------------------------------
$Gscript = $ENV{'SCRIPT_NAME'};
$Gscript = "${Gscript}.gd" if (! ($Gscript =~ /.gd$/));
$Ghost   = $ENV{'SERVER_NAME'};
$Gport   = $ENV{'SERVER_PORT'};



if ($0 =~ /.gpd/) {
    # We have gopher+ directory...
    $gplus = 1;
}

if (&check_access() eq '-') {
    print "+INFO: " if ($gplus);
    print "0Sorry! No access off of campus!\t$Ggw sorry\t$Ghost\t$Gport";
    exit;
}

if ($#ARGV > 0) {
    ($index, $cmd, $query) = @ARGV;
} else {
    $index = <>; chop($index);
    $cmd   = <>; chop($cmd);
    $query = <>; chop($query);

    $index = "default"         if ($index eq 'Word Titles');
    $index = "thesaurus"       if ($index eq 'Thesaurus');
    $index = "dictionary-full" if ($index eq 'Entire Dictionary');
    
    $cmd   = "SPELL"           if ($cmd eq 'Exact Match');
    $cmd   = "PSPELL"          if ($cmd eq 'Phonetic Match');
    $cmd   = "ENDINGS"         if ($cmd eq 'Words that start with...');
}

&GopenServer($webster_server,$webster_port);

&Gsend("INDEX $index") if ($index ne 'default');

if ($cmd eq "PSPELL") {                     # phonetic lookup
    &Gsend("SPELL $query ?");
} else {
    &Gsend("$cmd $query");
}

$_ = &Grecv;

if (/^SPELLING 0/ || /^WILD 0/) {
    exit;
} elsif (/^SPELLING 1/) {

    print "+INFO: " if ($gplus);
    print "0$query\texec:$index DEFINE $query:$Gscript\t$Ghost\t$$Gport\n"; 
    exit;
} elsif (/^SPELLING$/ || /^MATCHS$/ || /^WILD/) {
    $/ = "\200";
    $buf = <GSERVER>;
    $buf =~ s/[\r\200]//g;
    foreach (split(/\n/,$buf)) { 
	/\d+\s+(\S+)/; 
	$words{$1}=""; 
    }
    foreach (sort keys %words) { 
	print "+INFO: " if ($gplus);
	print "0$_\texec:$index DEFINE $_:$ENV{'SCRIPT_NAME'}.gd\t$ENV{'SERVER_NAME'}\t$ENV{'SERVER_PORT'}\n"; 
    }
} elsif (/^DEFINITION/) {
    $/ = "\200";
    $buf = <GSERVER>;
    $buf =~ s/[\r\200]//g;
    print $buf;
} else {
    exit;
}
&Gsend("QUIT");
&GcloseServer;
exit;

sub check_access {
    
    $ipaddress = $ENV{'REMOTE_ADDR'};
    return '+' if (!$ipaddress);
    foreach (@webster_acl) {
	($ipacl,$access)=split;
	return $access if  ($ipaddress =~ /$ipacl/);
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
    print "+INFO: " if ($gplus);
    print "0$mesg\t\terror.host\t70\r\n";
}

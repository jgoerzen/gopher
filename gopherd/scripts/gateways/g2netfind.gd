#!/usr/local/bin/perl
#
# This script allows you to add 'netfind' as a gopher menu item. To use
# this, make a link like this in your gopher tree:
#
#  Type=1?
#  Name=Netfind Search
#  Host=+
#  Port=+
#  Path=0/bin/gateways/g2netfind
#
# If you need to support old-stype clients you can do this instead
#
#  Type=7
#  Name=Netfind (non-gopher+)
#  Host=+
#  Port=+
#  Path=7/bin/gatewasy/g2netfind.gd
#
# You will need the netfind software, which you can get by ftp'ing to
# ftp.cs.colorado.edu
#
#----------------------------------------------------------------------
# variables you should change:

$dblookupcmd   = "/home/mudhoney/netfind/nf/dblookup";
$netfindcmd    = "/home/mudhoney/netfind/nf/netfind";

#----------------------------------------------------------------------
# gateway from gopher to netfind, requires netfind 3.46 or above

$SIG{'INT'}  = 'Ourabort';
$SIG{'TERM'} = 'Ourabort';
$SIG{'HUP'}  = 'Ourabort';
$SIG{'QUIT'} = 'Ourabort';
$SIG{'PIPE'} = 'Ourabort';
$SIG{'ALRM'} = 'Ourabort';

$| = 1;

if ($0 =~ /.gpd/) {
    # We have gopher+ directory...
    $gplus = 1;
}

if ($#ARGV > 0) {
    $name = shift;
    $domain = join(" ",  @ARGV);
} else {
    $name = <>; chop($query);
    $domain = <>; chop($domains);
}

if ((length($name) < 1) || (length($domain) < 1) ) {
    print "+INFO: " if ($gplus);
    print "0Please fill in both fields...\tmoo\tmoo\t70\n";
}

if ($name =~ /^\@\@/) {
    $cmd = "netfind";
    $name = substr($name, 2);;
} else {
    $cmd = "dblookup";
}

if ($cmd =~ /^dblookup/) {
    $name  =~ s/([^A-Za-z0-9])//g;
    $domain =~ s/([^A-Za-z0-9 ])//g;

    $script = $ENV{'SCRIPT_NAME'};
    $script = "$script.gd" if (!($script =~ /.gd$/));
    
    $dbcmd = "$dblookupcmd \"$domain\"";
    $pid = open(Dbfd, "$dbcmd |") || die "Cannot execute $dbcmd command...\n";

    select (DBfd);
    $| = 1;
    select(STDOUT);

    while (<Dbfd>) {
	chop;
	$title = $_;
	/^([^ ]+) .*/;
	$domain = $1;
	print "+INFO: " if ($gplus);
	print "0$title\texec:@@$name $domain:$script\t$ENV{'SERVER_NAME'}\t$ENV{'SERVER_PORT'}\n";
    }
    close(Dbfd);
} 

if ($cmd =~ /^netfind/) {
    
    $brokendomain = $domain;
    $brokendomain =~ s/\./ /g;
    
    print "Domain is $domain.., user is $name\n";
    $nfcmd = "$netfindcmd -L $domain $name $brokendomain </dev/null 2>&1";
    $pid = open(nffd, "$nfcmd|") || die "Unable to execute $nfcmd...\n";

    ### Make the output line-buffered
    select (nffd);
    $|=1;
    select (STDOUT);

    while (<nffd>) {
	chop;
	print "$_\r\n";
    }
    print "\r\n";
    close(nffd);
}



sub Ourabort {
    kill(SIGINT, $pid);
    kill(SIGKILL, $pid);
    exit(0);
}

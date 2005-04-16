#!/usr/local/bin/perl

#----------------------------------------------------------------------
# variables you should change:

$FTPgwhost = "gopher-gw.micro.umn.edu";
$FTPgwport = 70;

#----------------------------------------------------------------------

if ($0 =~ /.gpd/) {
    # We have gopher+ directory...
    $gplus = 1;
}

if (defined(@ARGV)) {
    ($ftptohost) = @ARGV;
} else {
    $ftptohost = <>; chop($ftptohost);
}

&GopenServer("$FTPgwhost", $FTPgwport);

&Gsend("ftp:${ftptohost}@/");

while (<GSERVER>) {
    last if /^\./;
    print "+INFO: " if ($gplus);
    print;
}

&GcloseServer;

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
}

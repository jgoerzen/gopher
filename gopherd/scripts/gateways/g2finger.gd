#!/usr/local/bin/perl

#----------------------------------------------------------------------

if ($0 =~ /.gpd/) {
    # We have gopher+ directory...
    $gplus = 1;
}

if (defined(@ARGV)) {
    $host = shift;
    $host =~ /^(.*)@(.*)$/;
    $user = $1 if ($1);
    $host = $2 if ($2);
} else {
    $user = <>;    chop($user);
    $host = <>;    chop($host);
}

### Make sure the user is letters or numbers only...
$user =~ s/[^A-z0-9]//g;

&GopenServer($host, 79);
&Gsend($user) if ($user);
&Gsend("\r\n");

while(<GSERVER>) {
    s/\n//;
    s/\r//;
    push(@lines,$_);
}
    
print "+INFO: " if ($gplus);
print "0Finger output for $host\t$query\t$host\t79\n";
    
#
# Parse out various finger formats
#

# Unix style finger format, and gnu finger

if ($lines[0] =~ /Login\s+Name/ || $lines[0] =~ /User\s+Real/) {
    
    foreach (@lines) {
	if (!/^Login\s+Name/) {
	    /^([\S]+)/;
	    substr($_, 75) = "";
	    print "+INFO: " if ($gplus);
	    print "0$_\t$1\t$host\t79\n";
	} else {
	    print "+INFO: " if ($gplus);
	    print "i$_\tnothing\tbogus\t0\n";
	}
    }
}

#
# VMS finger format
#
if ($lines[1] =~ /Jobs.*Load ave/) {
    foreach (@lines) {
	if (/^ User\s+Personal Name/ || /\d\d:\d\d:\d\d$/ ||
	    /Jobs\s+Load ave/) {
	    print "+INFO: " if ($gplus);
	    print "i$_\tnothing\tbogus\t0\n";
	} else {
	    /^([\S]+)/;
	    substr($_, 75) = "";
	    print "+INFO: " if ($gplus);
	    print "0$_\t$1\t$host\t79\n";
	}
    }
}

#
# CSO gateway finger type servers
#
    
foreach (@lines) {
    if (/\s+name:\s+(.*)/ ||
	/\s+person:\s+(.*)/) {
	$newquery = $1;
	$newquery =~ s/,//g; #remove commas
	$newquery =~ s/\s+[A-z]$//; #remove trailing middle initial
	print "+INFO: " if ($gplus);
	print "0$1\t$newquery\t$host\t79\n";
    }
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
}

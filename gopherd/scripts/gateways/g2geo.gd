#!/usr/local/bin/perl

#----------------------------------------------------------------------
# variables you should change:

$geo_server    = "geoserver.eecs.umich.edu";
$geo_port      = 3000;

#----------------------------------------------------------------------

# gateway from gopher to the geographic name server
    
if ($0 =~ /.gpd/) {
    # We have gopher+ directory...
    $gplus = 1;
}

if (defined(@ARGV)) {
    $query = join(' ', @ARGV);
} else {
    $query = <>;
    chop($query);
}

    
$action = $query;

if ($action =~ /^@/) {
    $city = $action;
    $city =~ s/^@@//;
    &GopenServer($geo_server, $geo_port);
    
    # remove banner
    while ($_ ne ".") {
	$_ = &Grecv;
    }
    &Gsend($city);
    
    $_ = "";
    
  LOOP:
    while ($_ ne "." || $_ eq "") {
	
	$_ = &Grecv;
	
	s/^R /Note            : /;
	s/^0 /City            : /;
	if (/^1 /) {
	    s/^1 //;
	    s/(\d+)/[$1]/;
	    push(@Counties,$_);
	    next LOOP;
	}
	s/^3 /Nation          : /;
	s/^A /Area-Code       : /;
	s/^2 /State\/Province  : /;
	s/^F /Feature Code    : /;
	
	if (/^E /) {
	    s/^E /Elevation       : /;
	    $_ = $_ . " feet";	# 
	}
	if (/^L /) {
	    ($L, $latdeg, $latmin, $latsec, $NS,
	     $londeg, $lonmin, $lonsec, $EW) =
		 split;
	    print "\n";
	    print "Latitude        : $latdeg degrees, $latmin minutes, $latsec seconds $NS\n";
	    print "Longitude       : $londeg degrees, $lonmin minutes, $lonsec seconds $EW\n";
	    print "\n";
	    next LOOP;
	}
	
	if (/^P /) {
	    s/^P /1980 Population : /;
	}
	
	if (/^Z /) {
	    s/^Z //;
	    push(@Zips, $_);
	    next LOOP;
	}
	
	print $_ . "\n" unless ($_ eq "." || $_ eq "");
    }
    
    print "\n";
    $line = "ZIP Codes       : ";
    foreach $i (@Zips) {
	if (length($line) <70) {
	    $line = "$line $i";
	} else {
	    print "$line\n";
	    $line = "                : ";
	}
    }
    print "$line\n\n";
    
    $line = "Counties        : ";
    foreach $i (@Counties) {
	if (length($line) <70) {
	    $line = "$line $i";
	} else {
	    print "$line\n";
	    $line = "                : ";
	}
    }
    
    print "$line\n";
    
    if ($_ eq "") {
	@Zips = ();
	@Counties = ();
	print "\n";
	next LOOP;
    }
}

### Do a search

else {
    $script = $ENV{'SCRIPT_NAME'};
    $script = "$script.gd" if (!($script =~ /.gd$/));

    &GopenServer($geo_server,$geo_port);
    # remove banner
    while ($_ ne ".") {
	$_ = &Grecv;
    }
    &Gsend($query);

    $_ = "";	
    while ($_ ne ".") {
	$_ = &Grecv;
	
	if (/^0 (.*)$/) {
	    $City = $1;
	}
	if (/^2 (\S+) (.*)$/) {
	    $States{$1} = $City;
	}
    }
    
    foreach $i (keys(%States)) {
	print "+INFO: " if ($gplus);
	print "0$States{$i}, $i\texec:\@$States{$i}, $i:$script\t$ENV{'SERVER_NAME'}\t$ENV{'SERVER_PORT'}\n";
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

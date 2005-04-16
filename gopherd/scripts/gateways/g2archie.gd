#!/usr/local/bin/perl

#----------------------------------------------------------------------
#variables you should change, note that archie server is set dynamically below:

@archies = ("archie.unl.edu", 
	    "archie.sura.net", 
	    "archie.rutgers.edu", 
	    "archie.rediris.es"
	    );
		 

$archie_prog   = "/usr/local/bin/archie";
$ftp_gateway   = "gopher-gw.micro.umn.edu";
$ftp_port      = 70;

@hqx_types     = ('.hqx');
@sound_types   = ('.snd',  '.au');
@image_types   = ('.gif', '.tiff', '.pcx', '.jpg');
@movie_types   = ('.mov', '.mpg');
@html_types    = ('.htm','.html');
@binary_types  = ('.exe', '.tar.Z', '.zip', '.zoo', '.arj', '.arc',
		  '.lzh', '.hyp', '.pak', '.exe', '.com');

#----------------------------------------------------------------------

# gateway from gopher to archie
# this is the "brute force" kind of approach; a tidier solution
# would speak the prospero protocols directly.

$SIG{'INT'}  = 'Ourabort';
$SIG{'TERM'} = 'Ourabort';
$SIG{'HUP'}  = 'Ourabort';
$SIG{'QUIT'} = 'Ourabort';
$SIG{'PIPE'} = 'Ourabort';
$SIG{'ALRM'} = 'Ourabort';


if ($0 =~ /.gpd/) {
    # We have gopher+ directory...
    $gplus = 1;
}

$| = 1;

#
# Default values for gopher0 clients...
#
$qtype = "Exact";
$maxhits = 100;

if (defined(@ARGV)) {
    ($query, $maxhits, $qtype) = join(' ', @ARGV);
} else {
    $query   = <>; chop($query);
    $qtype   = <>; chop($qtype);
    $maxhits = <>; chop($maxhits);
}

$arg = '-s' if ($qtype eq 'Substring');
$arg = '-e' if ($qtype eq 'Exact');
$arg = '-r' if ($qtype eq 'Regular Expression');

$numhits = "-m $numhits" if ($numhits =~ /^\d+$/);


$query =~ s/([^A-Za-z0-9])/\\$1/g;


exit if (length($query) <1);

print "+INFO: " if ($gplus);
print "iStarting Archie Search, patience.....\t\t\t70\n";
# 
$archie_server = &find_fastest_server(@archies);

print "+INFO: " if ($gplus);
print "iSearching Archie server at $archie_server\t\t\t70\n";
	
$archcmd = "$archie_prog -l -t $arg $numhits -h $archie_server $query";

$pid = open(archfd, "$archcmd|") || die "Could not execute $archcmd...\n";
select(archfd);
$| = 1;
select(STDOUT);


while (<archfd>) {
    ($time, $size, $host, $file) = split;;
    $type = 0;			# assume text.
    $type = "1" if ($file =~ m./$.) ;

    foreach $i (@binary_types) {
	$type = "9" if ($file =~ /$i$/i);
    }
    foreach $i (@image_types) {
	$type = "I" if ($file =~ /$i$/i);
    }
    foreach $i (@movie_types) {
	$type = ":" if ($file =~ /$i$/i);
    }
    foreach $i (@sound_types) {
	$type = "s" if ($file =~ /$i$/i);
    }
    foreach $i (@hqx_types) {
	$type = "4" if ($file =~ /$i$/i);
    }
    foreach $i (@html_types) {
	$type = "h" if ($file =~ /$i$/i);
    }
    
    print "+INFO: " if ($gplus);
    print "$type$host:$file\tftp:$host@$file\t$ftp_gateway\t$ftp_port\n";
    if ($gplus) {
	print "+URL:\n";
	print " ftp://$host$file\n";
    }
}

#
# This routine tries to find the least loaded Archie server
# based on Richard Perlman code from Pacific Bell
#

sub find_fastest_server {
    local(@sites) = @_;

    $ServerMinLoad = 9999999;

    open(moo, "/usr/ucb/rup @sites|");
    while (<moo>) {
	m/^[ \t]*([^: \t]+).*load average: ([0-9\.^ ]*)/;
	$abbrevsite = $1 ; 
	$itsload=$2;
	foreach $i (@sites) {
	    if ($abbrevsite eq substr($i,0,12)) {
		$fullsite=$i;
	    }
	}
	$possible_load{"$fullsite"} = $itsload;
	if ($itsload < $ServerMinLoad) {
	    $ServerMinLoad = $itsload;
	    $ServerMinHost = $fullsite;
	}
    }    
    return ($ServerMinHost);
}


sub Ourabort {
    kill(SIGINT, $pid);
    kill(SIGKILL, $pid);
    exit(0);
}

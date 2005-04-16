#!/usr/local/bin/perl
#
# This script allows you to search a WHOIS directory via gopher
# To use this, make a link like this in your gopher tree:
# 
#  Type=1?
#  Name=WHOIS search
#  Host=+
#  Port=+
#  Path=0/bin/gateways/g2whois
#
# If you need to support old-style clients you can do this instead
#
#  Type=7
#  Name=WHOIS search (non-gopher+)
#  Host=+
#  Port=+
#  Path=7/bin/gateways/g2whois.gd
#
#----------------------------------------------------------------------
# variables you may want to change change:

$whois_server = "whois.internic.net";
$whois_port   = 43;

#----------------------------------------------------------------------

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

if ($query =~ /^$/) {
    print "+INFO: " if ($gplus);
    print "0Nothing specified for search, please try again..\tmoo\terror.host\t70\n";
    exit;
}

#
# Always return the output of the whois search first
#

print "+INFO: " if ($gplus);
print "0All records containing \"$query\"\t$query\t$whois_server\t$whois_port\n";

&GopenServer($whois_server,$whois_port);
&Gsend($query);

while(<GSERVER>) {
    s/\n//;
    s/\r//;
    push(@lines,$_);
}




#
# Test for some common formats
#

#
# CSO gateway whois type servers
#

foreach (@lines) {
    if (/\s+name:\s+(.*)/ ||
	/\s+person:\s+(.*)/) {
	$newquery = $1;
	$newquery =~ s/,//g; #remove commas
	$newquery =~ s/\s+[A-z]$//; #remove trailing middle initial
	print "+INFO: " if ($gplus);
	print "0$1\t$newquery\t$whois_server\t$whois_port";
    }
}

# 
# nic.ddn.mil type whois servers
#

foreach (@lines) {
    if (/(.*)\((.*)\)\t(.*)$/) {
	print "+INFO: " if ($gplus);
	print "0$1\t!$2\t$whois_server\t$whois_port\n";
    }
}

#
# format used by stanford.edu
#    
if ($lines[$#lines] =~ /^\(returned/) {
    foreach (@lines) {
	if (/(.*)<(.*)>.*\s+(.*)$/) {
	    $text=$1; $handle=$2; $rest = $3;
	    $text =~ s/\s+$//;
	    $rest =~ s/\s+/ /g;
	    print "+INFO: " if ($gplus);
	    print "0$text ($rest)\t$handle\t$whois_server\t$whois_port";
	}
    }
}

#
# Format used at whohost.uwo.ca Try to handle both long form and short
# forms.  Long is a series of <field-name>: <field-value> pairs
# Short format: two lines with name on first and department indented
# on the second.

if ($lines[0] =~ /^There were [0-9]+ matches on your request\./ ||
    $lines[0] =~ /^\s+Full\sName: .*$/) {
    foreach (@lines) {
	if (/:/) {		# must be full form
	    if (/^\s+Full\sName: (.*)$/) {      #the name field
		$fullname = $1;
	    }
	    if (/^\s+Department: (.*)$/) {      # department name
		$department = $1;
	    }
	    if (/^\s+Index Key: (.*)$/) {       # index number
		$newquery = $1;
		print "+INFO: " if ($gplus);
		print "0$fullname  [$department]\tkey$newquery\t$whois_server\t$whois_port\n";
	    }
	}
	else { #short form (first line begins with <space> or *
	    if (/^[ *]([A-z0-9].*)\[(.*)\]/) {
		$newquery = $2;              #grab index
		$fullname= $1;               # and the name for display
	    }
	    if (/^  (.*)     /) {     # on the second line
		$department = $1;            #get department
		$department =~ s/ +$//;  #delete trailing spaces
		print "+INFO: " if ($gplus);
		print "0$fullname  [$department]\tkey$newquery\t$whois_server\t$whois_port\n";
	    }
	}
    }
}


#
# format used by X.500 gateways.
#
if ($lines[0] =~ m/[0-9]+ matches were found/||
    $lines[1] =~ m/[0-9]+ matches were found/) {
    foreach (@lines) {
	if (/X.500 Common Name.*:\s*(.*)\s*$/) {
	    $newname = $1;
	    print "+INFO: " if ($gplus);
	    print "0$newname\t$newname\t$whois_server\t$whois_port\n";
	}
	elsif (/^\s+[0-9]+\.(.*)\s+   (.*)$/) {
	    $newname = $1;
	    $newname =~ s/^\s+//;
	    $newname =~ s/\s+$//;
	    print "+INFO: " if ($gplus);
	    print "0$newname ($2)\t$newname\t$whois_server\t$whois_port\n";
	}
    }
}

#
# Format used by sunysb.edu, software unknown.
#
if ($lines[0] =~ /^Connection received/) {
    foreach (@lines) {
	if (/^(.*)MailName:/) {
	    print "+INFO: " if ($gplus);
	    print "0$1\t$1\t$whois_server\twhois_port\n";
	}
    }
}

#
# format used by horton: username@host  Name  Date
#

foreach (@lines) {
    if (/([\S]+@[\S]+).*([A-z][A-z][A-z] [A-z][A-z][A-z]\d\d \d\d\d\d)[\s]+$/) {

	;			# Not implemented yet...
    }
}


foreach (@lines) {
    if (/^\s+name:\s+(.*)/) {
	print "+INFO: " if ($gplus);
	print "0$1\t$query\t$whois_server\t$whois_port\n";
	exit(0);
    }
}
# 
if ($lines[$#lines] eq 'NO MATCH') {
    print "+INFO: " if ($gplus);
    print "0No Match Was Found!\thelp\t$gopher_server\t$gopher_port\n";
} elsif ($lines[$#lines] =~ /^\(returned/) {
    foreach (@lines) {
	if (/(.*)<(.*)>.*\s+(.*)$/) {
	    $text=$1; $handle=$2; $rest = $3;
	    $text =~ s/\s+$//;
	    $rest =~ s/\s+/ /g;
	    print "+INFO: " if ($gplus);
	    print "0$text ($rest)\t!$handle\t$whois_server\t$whois_port\n";
	}
    }
}

exit(0);



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

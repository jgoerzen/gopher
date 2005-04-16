#######################################################################
# $Author: jgoerzen $
# $Revision: 1.1 $
# $Date: 2000/08/19 00:28:56 $
# $Source: /home/jgoerzen/tmp/gopher-umn/gopher/head/gopherd/scripts/Makefile,v $
# $State: Exp $
#
# Paul Lindner, University of Minnesota DCS.
#
# Copyright 1994 by the Regents of the University of Minnesota
# see the file "Copyright" in the distribution for conditions of use.
#######################################################################
# MODULE: Makefile for scripts
#######################################################################
# Revision History:
# $Log: Makefile,v $
# Revision 1.1  2000/08/19 00:28:56  jgoerzen
# Initial revision
#
# Revision 1.3  1995/02/17  18:30:00  lindner
# Fix install for go4gw-type scripts
#
# Revision 1.2  1994/12/11  18:07:39  lindner
# More intelligent install routines
#
#
#######################################################################


SCRIPTFILES = add-account add-account.ask

include ../../Makefile.config

install:
	@-if [ ! -d $(SERVERDATA) ]; then \
		echo "No scripts installed, no such directory $(SERVERDATA)";\
	else \
		mkdir $(SERVERDATA)/bin ;\
		cp $(SCRIPTFILES) $(SERVERDATA)/bin ;\
		cp -r gateways $(SERVERDATA)/bin ;\
		ln -s $(SERVERDATA)/bin/gateways/g2archie.gd $(SERVERDATA)/bin/gateways/g2archie.gpd ;\
		ln -s $(SERVERDATA)/bin/gateways/g2finger.gd $(SERVERDATA)/bin/gateways/g2finger.gpd ;\
		ln -s $(SERVERDATA)/bin/gateways/g2ftphack.gd $(SERVERDATA)/bin/gateways/g2ftphack.gpd ;\
		ln -s $(SERVERDATA)/bin/gateways/g2geo.gd $(SERVERDATA)/bin/gateways/g2geo.gpd ;\
		ln -s $(SERVERDATA)/bin/gateways/g2netfind.gd $(SERVERDATA)/bin/gateways/g2netfind.gpd ;\
		ln -s $(SERVERDATA)/bin/gateways/g2nntp.gd $(SERVERDATA)/bin/gateways/g2nntp.gpd ;\
		ln -s $(SERVERDATA)/bin/gateways/g2webster.gd $(SERVERDATA)/bin/gateways/g2webster.gpd ;\
		ln -s $(SERVERDATA)/bin/gateways/g2whois.gd $(SERVERDATA)/bin/gateways/g2whois.gpd ;\
	fi

clean:
	-rm -f $(OBJS) *.out *~ core


include ../Makefile.config

GFILT = ../gophfilt/gophfilt -h localhost -s 9999 -p 

test-nonroot:
	@echo "Building test.conf file"
	@-rm -f test.conf
	@cat <test.raw >test.conf
	@echo "auxconf: /auxconf `pwd`/otheradmin.conf" >>test.conf
	@echo "Starting test gopher server on port 9999"
	../gopherd/gopherd -C -o test.conf -c `pwd`/data 9999
	@sleep 5

	-@rm -f ./results/root ./results/root-long \
		./results/links ./results/links-long
	$(GFILT) "" -t 1         >./results/root
	$(GFILT) "" -i '$$' -t 1 >./results/root-long
	$(GFILT) "" -i '!' -t 1  >./results/root-info

	$(GFILT) "1/links" -t 1         >./results/links
	$(GFILT) "1/links" -i '$$' -t 1 >./results/links-long
	$(GFILT) "1/links" -i '!' -t 1  >./results/links-info
	
	$(GFILT) "1/auxconf" -t 1         >./results/aux
	$(GFILT) "1/auxconf" -i '$$' -t 1 >./results/aux-long
	$(GFILT) "1/auxconf" -i '!' -t 1  >./results/aux-info

	$(GFILT) "1/views" -t 1         >./results/views
	$(GFILT) "1/views" -i '$$' -t 1 >./results/views-long
	$(GFILT) "1/views" -i '!' -t 1  >./results/views-info
	$(GFILT) "0/views/t-shirt" -t 0  >./results/views-txt

	$(GFILT) "0/decode-n-scripts/C-Programming" -t 0  >./results/views-txt
	$(GFILT) "m/cowmail" -t 1 >./results/mailfile
	$(GFILT) "m/fileseparator" -t 1 >./results/filesep	

all: test-nonroot
	@echo "Now type make diffs to find any differences"

	
diffs:
	-@diff -c good-results results




#!/usr/local/bin/perl 
# 

while(<STDIN>){ 
     @_ = split; 
     print "first item =\t",$_[0],"\n"; 
     print "second item =\t", $_[1],"\n"; 
     print "third item =\t", $_[2],"\n"; 
     print "all items =\t", @_, "\n"; 
     print "all items =\t@_ \n"; 
     print 'all items =\t@_ \n'; 
     print "\n"; 
} 

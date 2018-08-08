
#!/usr/local/bin/perl 
#  

while(<STDIN>){ 
     @data = split(/\s+/,$_); 
     print "first item =\t",$data[0],"\n"; 
     print "second item =\t", $data[1],"\n"; 
     print "third item =\t", $data[2],"\n"; 
     print "all items =\t", @data, "\n"; 
     print "all items =\t@data \n"; 
     print 'all items =\t@data \n'; 
     print "\n"; 
} 

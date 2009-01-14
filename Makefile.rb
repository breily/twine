comment "Makefile for the Twine threading library"
comment "$ make all     # Build library + test program"
comment "$ make lib     # Build library (thread.o)"

vars :FLAGS   => "-D__MAKECONTEXT_V2_SOURCE -Wall",
     :OFILES  => ["thread.o", "twine.o"],
     :OUTFILE => "twine"

rule :all, :d => :OFILES do
    compile :o => :OUTFILE
end

rule :lib do
    compile :obj, :i => "thread.c", :o => "thread.o"
end

clean :OFILES, :OUTFILE

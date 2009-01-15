comment "Makefile for the Twine threading library"
comment "$ make all     # Build library + test program"
comment "$ make lib     # Build library (thread.o)"

vars :FLAGS => "-D__MAKECONTEXT_V2_SOURCE -Wall"

rule "all", :d => ["simplemutex", "simplethreads"]

rule "simplemutex", :d => ["thread.o", "simplemutex.o"] do
    compile :o => "simplemutex"
end

rule "simplethreads", :d => ["thread.o", "simplethreads.o"] do
    compile :o => "simplethreads"
end

rule :lib do
    compile :obj, :i => "thread.c", :o => "thread.o"
end

clean "thread.o", "simplethreads.o", "simplemutex.o",
      "simplethreads", "simplemutex"

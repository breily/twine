comment "Makefile for the Twine threading library"
comment "$ make all     # Build library + test programs"
comment "$ make lib     # Build library (thread.o)"

vars :FLAGS => "-D__MAKECONTEXT_V2_SOURCE -Wall"

rule "all", :d => ["mutex_test", "thread_test"]

rule "mutex_test", :d => ["thread.o", "mutex_test.o"] do
    compile :o => "mutex_test"
end

rule "thread_test", :d => ["thread.o", "thread_test.o"] do
    compile :o => "thread_test"
end

rule :lib do
    compile :obj, :i => "thread.c", :o => "thread.o"
end

clean "thread.o", "thread_test.o", "mutex_test.o",
      "thread_test", "mutex_test"



vars :FLAGS   => "-D__MAKECONTEXT_V2_SOURCE -Wall",
     :OFILES  => ["thread.o", "stringtest.o"],
     :OUTFILE => "thread"

rule :all, :d => :OFILES do
    compile :o => :OUTFILE
end

rule :lib do
    compile :obj, :i => "thread.c", :o => "thread.o"
end

clean :OFILES, :OUTFILE

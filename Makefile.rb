

vars :FLAGS   => "-D__MAKECONTEXT_V2_SOURCE -Wall",
     :OFILES  => ["string.o", "stringtest.o"],
     :OUTFILE => "string"

rule :all, :d => :OFILES do
    compile :o => :OUTFILE
end

rule :lib do
    compile :obj, :i => "string.c", :o => "string.o"
end

clean :OFILES, :OUTFILE

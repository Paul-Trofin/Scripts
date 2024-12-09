# MAKEFILE TO COMPILE PYTHIA8
%: %.cc
	g++ -I/home/paul/MG5_aMC_v2_9_21/HEPTools/pythia8/include `root-config --cflags` $< -o $@ -L/home/paul/MG5_aMC_v2_9_21/HEPTools/pythia8/lib `root-config --glibs` -lpythia8

# make clean
clean:
	rm -f $(basename $(wildcard *.cc))

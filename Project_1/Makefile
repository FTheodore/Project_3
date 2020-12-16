vpath %.h ./Common ./Algorithms ./Clustering/ ./Structures ./Added_files
vpath %.cpp ./Common ./Algorithms ./Clustering/ ./Structures ./Added_files ./Mains .

CC =			g++
FLAGS = 		-c -std=c++11
OBJS_CLSTRNG =	algorithm.o Assignment.o Cluster.o Config.o Silhouette.o UtilsCluster.o
OBJS_CMMN =		bucket.o CmdArgumentsReader.o dataset.o Distance.o hashFuncs.o image.o PriorityQueue.o Utils.o
OBJS_STRCT =	HyperCube.o lsh.o
OBJS_ALG = 		AproxNN.o ExactNN.o RangeSearch.o
OBJS_EMD =		Labels.o ImgClusters.o Emd.o ExactNN.o
EXECS =			lsh cube cluster search_reduced search_emd

all: $(EXECS)

lsh: lsh_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG)
	$(CC) *.o -o $@
	rm $<

search_reduced: search_reduced_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG)
	$(CC) *.o -o $@
	rm $<

search_emd: search_emd_main.o $(OBJS_CMMN) $(OBJS_EMD)
	$(CC) *.o -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools
	rm $<

cube: cube_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG) 
	$(CC) *.o -o $@
	rm $<

cluster: cluster_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG) $(OBJS_CLSTRNG)
	$(CC) *.o -o $@
	rm $<

search_emd_main.o: ./Mains/search_emd_main.cpp
	$(CC) $(FLAGS) $< -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools

Emd.o: ./Added_files/Emd.cpp ./Added_files/Emd.h
	$(CC) $(FLAGS) $< -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools

%.o: %.cpp %.h
	$(CC) $(FLAGS) $< -o $@

%.o: %.cpp
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -rf $(EXECS) *.o

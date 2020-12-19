vpath %.h ./Common ./Algorithms ./Clustering/ ./Structures ./Added_files
vpath %.cpp ./Common ./Algorithms ./Clustering/ ./Structures ./Added_files ./Mains .

CC =			g++
FLAGS = 		-c -std=c++11
OBJS_CLSTRNG =	algorithm.o Assignment.o Cluster.o Config.o Silhouette.o UtilsCluster.o Objective.o
OBJS_CMMN =		bucket.o CmdArgumentsReader.o dataset.o Distance.o hashFuncs.o image.o PriorityQueue.o Utils.o
OBJS_STRCT =	HyperCube.o lsh.o
OBJS_ALG = 		AproxNN.o ExactNN.o RangeSearch.o
OBJS_EMD =		Labels.o ImgClusters.o Emd.o ExactNN.o
EXECS =			search_reduced search_emd cluster

all: $(EXECS)

search_reduced: search_reduced_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG) $(OBJS_EMD)
	$(CC) *.o -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools
	rm $<

search_emd: search_emd_main.o $(OBJS_CMMN) $(OBJS_EMD)
	$(CC) *.o -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools
	rm $<

cluster: cluster_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG) $(OBJS_CLSTRNG) $(OBJS_EMD)
	$(CC) *.o -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools
	rm $<

%.o: %.cpp %.h
	$(CC) $(FLAGS) $< -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools

%.o: %.cpp
	$(CC) $(FLAGS) $< -o $@ -I or-tools/include/ -L or-tools/lib/ -lortools

clean:
	rm -rf $(EXECS) *.o

vpath %.h ./Common ./Algorithms ./Clustering/ ./Structures
vpath %.cpp ./Common ./Algorithms ./Clustering/ ./Structures ./Mains .

CC =			g++
FLAGS = 		-c -std=c++11
OBJS_CLSTRNG =	algorithm.o Assignment.o Cluster.o Config.o Silhouette.o UtilsCluster.o
OBJS_CMMN =		bucket.o CmdArgumentsReader.o dataset.o Distance.o hashFuncs.o image.o PriorityQueue.o Utils.o
OBJS_STRCT =	HyperCube.o lsh.o
OBJS_ALG = 		AproxNN.o ExactNN.o RangeSearch.o
EXECS =			lsh cube cluster search

all: $(EXECS)

lsh: lsh_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG)
	$(CC) *.o -o $@
	rm $<

search: search_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG)
	$(CC) *.o -o $@
	rm $<

cube: cube_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG) 
	$(CC) *.o -o $@
	rm $<

cluster: cluster_main.o $(OBJS_CMMN) $(OBJS_STRCT) $(OBJS_ALG) $(OBJS_CLSTRNG)
	$(CC) *.o -o $@
	rm $<

%.o: %.cpp %.h
	$(CC) $(FLAGS) $< -o $@

%.o: %.cpp
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -rf $(EXECS) *.o

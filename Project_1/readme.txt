Κ23γ: Ανάπτυξη Λογισμικού για Αλγοριθμικά Προβλήματα

1η Προγραμματιστική Εργασία

Ομάδα 16
Αναστάσιος Αντωνόπουλος - 1115201400014
Θεόδωρος Φιλιππίδης - 1115201500170

Github link: https://github.com/tassos96/Project_1

Compilation Instructions: 
	Η μεταγλώττιση πραγματοποιείται μέσω makefile(separate compilation), 
	για την μεταγλώττιση δώστε την εντολή "make", θα δημιουργηθούν τα τρία εκτελέσιμα:
		-lsh 
		-cube 
		-cluster
	Υπάρχει και η εντολή "make clean" για την διαγραφή των αντικειμενικών αρχείων.

Usage: 

	LSH: "./lsh [–d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>]"

	CUBE: "./cube [–d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file> -Ν <number of nearest> -R <radius>]"

	CLUSTER: "./cluster [–i <input file> –c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>]"

	Όλα τα command line arguments είναι προαιρετικά, τα ονόματα των αρχείων αν δεν δοθούν 
	θα ζητηθούν κατά το runtime ενώ στις άλλες παραμέτρους θα δοθούν οι αντίστοιχες deafult τιμές.

	Σημείωση: 
		Στο clustering έχει προστεθεί ένα επιπλέον config file "parameters.conf" στο 
		οποίο δίνονται τιμές στις εξής παραμέτρους:

			clust_threshold: 
				Συνθήκη τερματισμού στο clustering, δηλαδή αν σε ένα iteration του αλγορίθμου έχουν γίνει
				reassign σε καινούργιο cluster το πολύ "clust_threshold" instances(δηλαδή images) 
				ο αλγόριθμος τερματίζει.
		
			w_smpl_prcnt: 
				Ποσοστό δείγματος από images του training dataset που θα χρησιμοποιηθούν στον προσεγγιστικό
				υπολογισμό του W, δηλαδή της μέσης απόστασης από τον κοντινότερο γείτονα.
		
			approx_threshold: 
				Συνθήκη τερματισμού στους Approximate Nearest Neighbour και Range Search αλγορίθμους
				όταν χρησιμοποιείται Locality Sensitive Hashing(LSH), δηλαδή αν ελεγχθεί ένα πλήθος 
				εικόνων αντίστοιχο αυτής της ποσότητας οι αλγόριθμοι τερματίζουν 
				και δεν συνεχίζεται η αναζήτηση στο bucket και στα άλλα lsh tables.
		
			w_factor: 
				Παράγοντας που πολλαπλασιάζεται με την μέση απόσταση από τον κοντινότερο γείτονα
				προκειμένου να ισχύει w >> r.
				
			clust_threshold_iters:
				Συνθήκη τερματισμού στο clustering, δηλαδή το πολύ clust_threshold_iters επαναλήψεις θα γίνουν
				από τον αλγόριθμο συσταδοποίησης

Γενική περιγραφή προγραμμάτων:
	Και στα τρία προγράμματα αρχικά διαβάζονται τα command line arguments και δημιουργούνται αντικέιμενα
	τύπου "Dataset" τα οποία περιέχουν vector με τις εικόνες που διαβάζονται από τα αρχεία train και query.
	Έπειτα δημιουργείται η απαραίτητη δομή, δηλαδή είτε Lsh είτε Hypercube (είτε καμία δομή σε
	περίπτωση του LLoyd αλγορίθμου για το clustering).

	Class Lsh:
		Το Lsh object αποτελείται από ένα vector με objects που αναπαριστούν τα Lsh tables(Class "LshTable").
		Τα Lsh Tables με την σειρά τους είναι σύνθεση ενός map που περιέχει δείκτες στα buckets του 
		Lsh Table, και μιας g hash function(Class "AmplifiedHash") η οποία με την σειρά της διατηρεί ένα vector με τις πιο
		απλές h hash functions(Class "SimpleHash"). Κατά την δημιουργία της δομής οι εικόνες του train
		set χωρίζονται στα buckets του κάθε Lsh Table, δηλαδή υπάρχει ένας δείκτης στο ίδιο image 
		ανά Lsh Table ο οποίος αποθηκεύεται στο αντίστοιχο bucket.

		Σημαντική Σημείωση:
			Κάθε φορά που μια εικόνα τοποθετείται σε ένα bucket η g hash function επιστρέφει δυο τιμές,
			το index του bucket και την original τιμή του hashing, η οποία διατηρείται στο bucket μαζί
			με τον δείκτη της εικόνας. Αυτό γίνεται για να χρησιμοποιηθεί αργότερα στους approximate
			αλγορίθμους έτσι ώστε να επιστρέφονται πραγματικά κοντινές στα query εικόνες.

	Class Hypercube:
		Το Hypercube object αποτελείται από ένα map που περιέχει δείκτες στους κόμβους δηλαδή τα buckets του 
		υπερκύβου και ένα vector που περιέχει τις f συναρτήσεις(Class "F") οι οποίες είναι υπεύθυνες για τις προβολές
		σε κάθε άξονα. Κατά την δημιουργία της δομής οι εικόνες του train set χωρίζονται στα buckets 
		του Hypercube. Οι f functions διατηρούν ένα object που αποτελεί την h hash function καθώς επίσης
		και ένα map με προηγούμενες τιμές που έχουν επιστραφεί από την f. Αυτό γίνεται για να επιστρέφεται
		πάντα η ίδια τιμή άπό την f όταν η h επιστρέφει την ίδια hash value για μια εικόνα.  

	Στο κώδικα περίεχονται επιπλέον αναλυτικά σχόλια για την λειτουργία και την υλοποίηση που ακολουθήθηκε.



Ο πηγαίος κώδικας έχει οργανωθεί με τα παρακάτω directories:

	Ο παρακάτω φάκελος περιέχει τις υλοποιήσεις των Nearest Neighbour (Exact και Approximate) και Range search αλγορίθμων.
	Algorithms/
	├── AproxNN.cpp
	├── AproxNN.h
	├── ExactNN.cpp
	├── ExactNN.h
	├── RangeSearch.cpp
	└── RangeSearch.h

	Ο παρακάτω φάκελος περιέχει κώδικα σχετικό με τη συσταδοποίηση.
	Clustering/
	├── algorithm.cpp	//Υλοποίηση του αλγορίθμου συσταδοποίησης.
	├── algorithm.h 	
	├── Assignment.cpp  //Υλοποίηση του βήματος ανάθεσης του αλγορίθμου συσταδοποίησης, 
	├── Assignment.h 	  το οποίο μπορεί να είναι είτε LLoyd είτε Reverse assign μέσω range search.
	├── Cluster.cpp 	//Περιέχει την κλάση που απαρτίζει μία συστάδα.
	├── Cluster.h
	├── Config.cpp		//Μέθοδος που διαβάζει το αρχείο "cluster.conf".
	├── Config.h
	├── Silhouette.cpp  //Μέθοδοι που υπολογίζουν τις τιμές Silhouette ανά εικόνα και ανά συστάδα.
	├── Silhouette.h
	├── UtilsCluster.cpp //Βοηθητικές μέθοδοι. 
	└── UtilsCluster.h

	Ο παρακάτω φάκελος περιέχει γενικό κώδικα που υλοποιεί βασικές λειτουργίες.
	Common/
	├── bucket.cpp		//Υλοποίηση κουβάδων/κόμβων για Lsh/Hypercube.
	├── bucket.h
	├── CmdArgumentsReader.cpp	//Μέθοδοι που διαβάζουν τα command line arguments των προγραμμάτων.
	├── CmdArgumentsReader.h
	├── dataset.cpp 	//Image container και μέθοδοι που διαβάζουν τις εικόνες από τα αρχεία train και test.
	├── dataset.h
	├── Distance.cpp 	//Υλοποίηση Manhattan Distance και υπολογισμός W.
	├── Distance.h
	├── hashFuncs.cpp 	//Συναρτήσεις κατακερματισμού h και g από τις διαλέξεις.
	├── hashFuncs.h
	├── image.cpp 		//Μοντελοποίηση μιας εικόνας. 
	├── image.h
	├── PriorityQueue.cpp //Ουρά προτεραιότητας για την αποθήκευση των κοντινότερων γειτόνων.
	├── PriorityQueue.h
	├── Utils.cpp 		//Βοηθητικές μέθοδοι. 
	└── Utils.h

	Ο παρακάτω φάκελος περιέχει τις main functions.
	Mains/
	├── cluster_main.cpp
	├── cube_main.cpp
	└── lsh_main.cpp

	Ο παρακάτω φάκελος περιέχει την μοντελοποίηση των Lsh και Hypercube.
	Structures/
	├── HyperCube.cpp
	├── HyperCube.h
	├── lsh.cpp
	└── lsh.h

	cluster.conf 	//Αρχείο config για το clustering.
	parameters.conf //Αρχείο config για γενικές παραμέτρους.
	Makefile 

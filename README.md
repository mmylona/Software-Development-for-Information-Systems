# **ΑΝΑΠΤΥΞΗ ΛΟΓΙΣΜΙΚΟΥ ΓΙΑ ΠΛΗΡΟΦΟΡΙΑΚΑ ΣΥΣΤΗΜΑΤΑ**
## **ΧΕΙΜΕΡΙΝΟ ΕΞΑΜΗΝΟ 2023–2024**


| Ονοματεπώνυμο                 | ΑΜ                  | Email                   |
| ---------------------------- | ------------------- | ------------------------|
| Μαρίνα Μυλωνά                | 1115201900229       | sdi1900229@di.uoa.gr   |
| Βαγγέλης Βραΐλας             | 1115201900314       | sdi1900314@di.uoa.gr   |
| Δέσποινα Χριστοδούλου        | 1115201900303       | sdi1900303@di.uoa.gr   |


Git repository: https://github.com/Vaggelisvl/Project_2023_1

<p align="center">
  <strong>Άσκηση 1</strong>
</p>


- **Σχεδιαστικές επιλογές:**<br>
Πιο κάτω παραθέτουμε την κύρια σχεδιαστική επιλογή, η οποία απεικονίζει το Γράφο. Συγκεκριμένα είναι ένα map, το οποίο έχει ως key την κλάση Point και ως value ένα vector από Neighbors. Οι υλοποιήσεις των custom δομών δεδομένων map και vector βρίσκονται στα αρχεία headers/structures/unorderedMap/UnorderedMap.h και headers/structures/vector/Vector.h αντίστοιχα.
Η κλάση Point αντιπροσωπεύει το κάθε στοιχείο του αρχείου που διαβάζουμε, η οποία έχει τις συντεταγμένες και το id του. H κλάση Neighbor είναι μια προέκταση του Point με τη διαφορά ότι έχει την απόσταση του από το key (point).


![image](https://github.com/Vaggelisvl/Project_2023_1/assets/139377786/1bf06e47-0a85-41a3-af03-b7e4fa275e3c)



- **Ανάγνωση από αρχείο: src/structures/Dataset.cpp**<br>
Η ανάγνωση από το αρχείο binary γίνεται στο Dataset.cpp, αφού έχει δώσει ο χρήστης το όνομα του αρχείου που θέλει να διαβάσει, τον αριθμό των διαστάσεων των σημείων και τον αριθμό των σημείων που υπάρχουν. Το τελευταίο όρισμα μπορεί να μην το δώσει, αν θέλει να διαβάσει από αρχεία με το format που μας δόθηκε γιατί θα το διαβάσει από το αρχείο.

- **Αρχικοποίηση Γράφου: src/structures/graph/GraphInitialization.cpp**<br>
Η μέθοδος setKRandomNeighbors εκτελεί τη διαδικασία τυχαίας αρχικοποίησης των Κ γειτόνων για κάθε σημείο στο Γράφο. Αφού βρει τον τυχαίο γείτονα στο σύνολο δεδομένων, υπολογίζει την απόσταση ανάμεσα στο τρέχον σημείο και τον τυχαίο γείτονα χρησιμοποιώντας τη συνάρτηση της ευκλείδειασ απόστασης. Δημιουργεί ένα αντικείμενο Neighbors για τον τυχαίο γείτονα με το ID του, την απόσταση και τις συντεταγμένες του. Τέλος το προσθέτει στο Γράφο και κάνει το ίδιο και για τον αντίστροφο του.

- **Αλγόριθμος KNN: src/structures/graph/GraphInitialization.cpp**<br>
Η μέθοδος KNNAlgorithm υλοποιεί τον αλγόριθμο K-Nearest Neighbors (KNN) για τη βελτιστοποίηση του συνόλου των γειτόνων για κάθε σημείο στο γράφο. Για κάθε σημείο στο γράφο ταξινομεί τους γείτονές του με βάση την απόσταση, έτσι ώστε ο γείτονας με τη μεγαλύτερη απόσταση να βρίσκεται στο τέλος του πίνακα. Στη συνέχεια για κάθε γείτονα αναζητά τους γείτονες γειτόνων και υπολογίζει την απόσταση του μεταξύ του τρέχοντος σημείου, αφού ελέγξει για διπλά σημεία. Εάν η απόσταση αυτή είναι μικρότερη από τη μέγιστη απόσταση που έχει καταγραφεί, τότε αντικαθίσταται ο extended γείτονας. Εφόσον έχει γίνει αλλαγή, η συνάρτηση επιστρέφει 0 για να ξανακληθεί η ίδια συνάρτηση, αλλιώς επιστρέφει 1.

- **Αλγόριθμος ΚΝΝ για σημείο εκτός Γράφου: src/structures/graph/GraphInitialization.cpp**<br>
Η μέθοδος findKNearestNeighborsForPoint, εκτελεί τον αλγόριθμο για την εύρεση των K-πλησιέστερων γειτόνων ενός query point που δεν βρίσκεται στο Γράφο. Αρχικοποιεί τυχαίους γείτονες υπολογίζοντας και τις αποστάσεις μεταξύ τους. Στη συνέχεια προσθέτει στο Γράφο το query point με τους Κ αυτούς γείτονες και καλεί τη πιο πάνω συνάρτηση(KNNAlgorithm). Όταν τελειώσει τον αλγόριθμο ΚΝΝ αφαιρεί το γράφο το σημείο αυτό.


- **Δυνατότητα χρήσης εναλλακτικής μετρικής ομοιότητας: src/structure/utils/Metrics.cpp**<br>
Έχουμε υλοποιήσει δύο μετρικές, την ευκλείδεια και την manhatan οι οποίες βρίσκονται στο αρχείο Metrics.cpp .

- **Dataset:**
1.	Το format Που μας δόθηκε(input1.cpp, 00000200.bin, 00001000.bin)
2.	Δικό μας format: 100 στοιχεία των 20 διαστάσεων(data.bin)

- **Results.txt:**<br>
Σε αυτό το αρχείο αποθηκεύονται τα αποτελέσματα από τους 20 πιο κοντινούς γείτονες κάθε σημείου του αρχείου. Χρησιμοποιείται μόνο για σκοπούς ελέγχου του αλγορίθμου ΚΝΝ.

- **Στο library.cpp υπάρχει η main**
- **Για compile:** make all 
- **Για compile χωρίς εγκατάσταση dependencies:** make all_skip_dependencies
- **Για τα τεστ:** make test 

- **Βελτιστοποιήσεις πρώτου μέρους:**<br>
•	Εμφάνιση αριθμού σωστών και λάθος αποφάσεων και ποσοστό συνολικού αριθμού σωστών Κ κοντινότερων γειτόνων του αλγόριθμου.
•	Πραγματοποίηση αλλαγών στο makefile
•	Υλοποίηση περαιτέρω tests
•	Διόρθωση αριθμών παραμέτρων που δέχεται το πρόγραμμα
•	Γενικές βελτιώσεις

<p align="center">
  <strong>Άσκηση 2</strong>
</p>

- **Κλάση Optimizations:**<br>
Στην κλάση αυτή βρίσκονται οι υλοποιήσεις της δεύτερης άσκησης, η οποία κληρονομεί την κλάση GraphInitialization. Η αρχικοποίηση του γράφου γίνεται με τον ίδιο τρόπο όπως προηγουμένως μέσω της συνάρτησης setKRandomNeighbors().

- **Local Join: src/structures/graph/Optimizations.cpp**<br>
 Καλείται ο αλγόριθμος KNN από την Library.cpp ο οποίος για κάθε σημείο στον Γράφο καλεί την local join συνάρτηση. Η συνάρτηση αυτή, για κάθε δυάδα γειτόνων, αρχικά ελέγχει με την συνάρτηση checkDuplicate() αν υπάρχουν στις λίστες γειτόνων των δύο αυτών σημείων ίδια id, μετρά την απόσταση μεταξύ τους και ελέγχει κατά πόσο πρέπει να ενταχθεί ο ένας γείτονας στην λίστα γειτόνων του άλλου.

- **Δυάδες αποστάσεων:**<br>
Κρατάμε μια δομή την hashMap η οποία έχει ως Key ένα point και ως value ένα struct(DistanceContents) με το id ενός άλλου point και την απόσταση μεταξύ τους. Αυτό γίνεται για να μην υπολογίζουμε δυο φορές την ίδια απόσταση μεταξύ δυο σημείων. Ο έλεγχος για το αν έχει υπολογιστεί ξανά μια απόσταση γίνεται στη συνάρτηση local join() με τη συνάρτηση hashingDuplicateDistnaces().

- **Incremental Search: src/structures/graph/Optimizations.cpp**<br>
Έχουμε προσθέσει στη δομή των Neighbors μια σημαία η οποία μετά την αρχικοποίηση του γράφου γίνεται true σε όλους τους γείτονες όλων των σημείων. Στην συνέχεια σε κάθε local join καλείται η συνάρτηση incrementalSearch() για να ελέγξει αν έστω ένα από τα δύο σημεία έχει flag true. Όταν προστίθεται ένα καινούριο σημείο ως γείτονας μέσα στο local join, η σημαία γίνεται false.

- **Δειγματοληψία: src/structures/graph/Optimizations.cpp**<br>
Για κάθε σημείο του γράφου καλείται η συνάρτηση sampling(). Η συνάρτηση αυτή υλοποιεί τη διαδικασία της δειγματοληψίας, στην οποία υπολογίζεται κάθε φορά το ποσοστό σημείων που θα υπολογιστούν σε ένα τοπικό join. Έπειτα για όλα τα σημεία του γράφου βρίσκει τους πρώτους percentageToUse γείτονες και αλλάζει μια μεταβλητή(setHasBeenChoosen) από 0 σε 1. Η μεταβλητή αυτή έχει προστεθεί στη δομή των Points και αρχικοποιείται για κάθε γείτονα σε 0.

- **Πρόωρος Τερματισμός: src/structures/graph/Optimizations.cpp**<br>
Η υλοποίηση του πρόωρου τερματισμού γίνεται με τη χρήση μιας μεταβλητής count σε κάθε επανάληψη, στην οποία καταγράφεται ο αριθμός των αλλαγών σε κάθε επανάληψη. Στο τέλος κάθε επανάληψης γίνεται έλεγχος αν το count είναι μικρότερο από το d*K, όπου το d είναι 0,01 και το K ο αριθμός των γειτόνων και τερματίζει ο αλγόριθμος. 

- **Εύρεση σημείου εκτός γράφου: src/structures/graph/Optimizations.cpp**<br>
Η διαδικασία είναι παρόμοια με την προηγούμενη άσκηση για τον αλγόριθμο σε σημείο εκτός γράφο, με τη διαφορά ότι κάθε σημείο που προστίθεται ως γείτονας στο σημείο ενημερώνεται και η σημαία σε true. Η υλοποίηση γίνεται στη συνάρτηση findKnearestNeighborsForGraph().

- **Αντίστροφες λίστες**:<br>
Οι αντίστροφες λίστες αποθηκεύονται σε ένα UnorderedMap με όνομα reverseNN και έχει την ίδια δομή με το γράφο. Μετά την αρχικοποίηση του γράφου καλείται η συνάρτηση initReverseNN() όπου αρχικοποιείται ο reverseNN. Επίσης κάθε φορά που προστίθεται ή αφαιρείται ένας γείτονας ανανεώνεται και ο reverseNN.

- **Αποτελέσματα:**
  1. graph.txt: Εκτυπώνεται ο γράφος της πρώτης άσκησης μετά τον αλγόριθμο
  2. optimizedGraph.txt: Εκτυπώνεται ο γράφος της δεύτερης άσκησης μετά τον αλγόριθμο
  3. reverseNN.txt: Εκτυπώνεται ο γράφος με τις αντίστροφες λίστες μετά τον αλγόριθμο
  4. results.txt: Εκτυπώνονται όλα τα σημεία με τους πιο κοντινούς γείτονες ταξινομημένους

- **Εκτέλεση Προγράμματος:**<br>
Με το make run εκτελείται default το src/input1.bin 100 20 5 1 euclidean 
(100 διαστάσεων, 20 σημεία, Κ=5, queryId=1, μετρική ευκλείδια)

- **Βελτιστοποιήσεις δεύτερου μέρους:**<br>
•Το πρόγραμμα δέχεται τα ορίσματα απο τη γραμμή εντολών με αυτή τη σειρά:<br>
<inputFile> <dimensions> <numOfPoints> <K> <queryId> <d> <metrics> <D> <numOfTrees> <numOfThreads>

<p align="center">
  <strong>Άσκηση 3</strong>
</p>

- **Εκτέλεση Προγράμματος:**<br>
Με το make run εκτελείται default το src/input1.bin 100 20 5 1 0.01 euclidean 3 10 5 
(100 διαστάσεων, 20 σημεία, Κ=5, queryId=1, d=0.01, μετρική ευκλείδια, D=3, 10 δέντρα, 5 threads)



# **ΤΕΛΙΚΗ ΑΝΑΦΟΡΑ**

## **Πειράματα:**

- **Αρχικοποίηση Γράφου**<br>

   ![image](https://github.com/Vaggelisvl/Project_2023_1/assets/139377786/c76b4359-052c-4b7f-8ebd-6605b49e6748)

Στο παραπάνω διάγραμμα βλέπουμε το χρόνο εκτέλεσης από τις 3 ασκήσεις που υλοποιήσαμε στην αρχικοποιήση του γράφου. Παρατηρούμε οτι η πιο χρονοβόρα αρχικοποίηση είναι της τρίτης άσκησης. Αυτό είναι λογικό καθώς σε αυτή την άσκηση κάναμε βελτιστοποιήσεις ως προς την αρχικοποίηση, χρησιμοποιώντας δέντρα τυχαίων προβολών και συγκρίνοντας τα σημεία στα φύλλα. Η διαδικασία αυτή δεν γινόταν στις άλλες δυο ασκήσεις αλλά γινόταν με εντελώς τυχαίους γείτονες. Οπότε, παρόλου που κάναμε βελτιστοποιήσεις σε αυτό το βήμα, δεν βλέπουμε κάποιο αποτέλεσμα σε αυτό το βήμα σε σχέση με το χρόνο.



- **KNN Αλγόριθμος**<br>

   ![image](https://github.com/Vaggelisvl/Project_2023_1/assets/139377786/c61726f1-a7c5-44ab-ade1-0b3b87a2f97c)

Στο παραπάνω διάγραμμα βλέπουμε το χρόνο εκτέλεσης για τον αλγόριθμο ΚΝΝ στις 3 ασκήσεις. Το μικρότερο χρόνο εκτέλεσης τον έχουμε στην πρώτη άσκηση, στην οποία συγκρίναμε τις αποστάσεις των σημείων μεταξύ γειτόνων και γειτόνων γειτόνων. Στην δεύτερη άσκηση όμως αλλάξαμε την οπτική μας και συγκρίναμε τις αποστάσεις μεταξύ των γειτόνων και για κάθε γείτονα ελέγχαμε αν πρέπει να προστεθέι στους κ κοντινότερους του άλλου. Επίσης είχαμε τη δειγματοληψια και κάποιους άλλους ελέγχους με αποτέλεσμα ο αλγόριθμος να αργεί. Στην τρίτη άσκηση ο αλγόριθμος αυτός γίνεται job έτσι ώστε να μοιραστεί η δουλειά στα threads και να έχουμε μικρότερο χρόνο εκτέλεσης απο τη δεύτερη. Το πείραμα αυτό έγινε με αριθμό νημάτων ίσο με 5. Τέλος, ο λόγος που η πρώτη άσκηση έχει μικρότερο χρόνο εκτέλεσης του αλγορίθμου από την τρίτη άσκηση, είναι γιατί στην τρίτη άσκηση καλείται ο αλγόριθμος της δευτερης και όχι της πρώτης. 


- **Χρόνος εκτέλεσης προγράμματος**<br>

  ![image](https://github.com/Vaggelisvl/Project_2023_1/assets/139377786/ded0561e-06ce-49c3-a2fd-bfb80f3308a5)

Στο παραπάνω διάγραμμα βλέπουμε τον χρόνο εκτέλεσης της κάθε άσκησης ξεχωριστά. Παρατηρούμε οτι το χειρότερο χρόνο τον έχει η δεύτερη άσκηση και αυτό είναι λογικό με βάση τα προηγούμενα πειράματα. Επίσης παρόλου που ο αλγόριθμος ΚΝΝ στη τρίτη άσκηση έχει μεγαλύτερο χρόνο από τη πρώτη, βλέπουμε ότι ο χρόνος εκτέλεσης της τρίτης δεν είναι πολύ μεγαλύτερος απο τη πρώτη. Αυτό σημαίνει οτι με τον πολυνημαντισμό καταφέραμε να μειώσουμε πολύ το χρόνο εκτέλεσης, σχεδόν όσο είναι και στην πρώτη.


- **Αριθμός νημάτων**<br>

   ![image](https://github.com/Vaggelisvl/Project_2023_1/assets/139377786/cf4498ba-7b68-4a5c-b8c0-5120bacebdf1)

  Στο παραπάνω πείραμα βλέπουμε το χρόνο εκτέλεσης της τρίτης άσκησης, δοκιμάζοντας τον αριθμό των νημάτων με 4,5 και 6. Τα πιο πάνω αποτελέσματα οδηγούν στο συμπέρασμα οτι για μικρό αριθμό σημείων και μικρό αριθμό γειτόνων δεν βλέπουμε κάποια ιδιαίτερη διαφορά στο χρόνο καθώς και οι 3 δοκιμές έχουν σχεδόν τον ίδιο χρόνο, με μια ελάχιστη διαφορά για 4 νήματα. Αντιθέτως στα 1000 σημεία με κ=20 και 500 σημεία με κ=25, βλέπουμε οτι με 5 νήματα εμφανίζει καλύτερα αποτελέσματα. Οπότε αποφασίσαμε οτι ο αριθμός 5 για τα νήματα είναι ιδανικός, αφού για για λιγότερα σημεία δεν βλέπουμε κάποια ιδιαίτερη διαφορά αλλά για πιο χρονοβόρες διαδικασίες δουλεύει καλύτερα.


- **Αριθμός δεντρών**<br>
![image](https://github.com/Vaggelisvl/Project_2023_1/assets/139377786/cad1feec-9fdb-4cc8-bf67-97a59b41cd82)

Στα παραπάνω διαγράμματα βλέπουμε το ποσοστό επιτυχίας, αλλά και το χρόνο εκτέλεσης του προγράμματος με διαφορετικό αριθμό δεντρών. Σύμφωνα με τα αποτελέσματα που βγάλαμε η δημιουργία 10 δεντρών μας δίνει τα καλύτερα αποτελέσματα, εκτός απο μια περίπτωση(500 σημεία και κ=10). Βλέποντας και το δεύτερο διάγραμμα, παρατηρήσαμε οτι οι χρόνοι δεν έχουν μεγάλη διαφορά μεταξύ τους, οπότε αποφασίσαμε πως η καλύτερη επιλογή για τον αριθμό των δέντρων τυχαίας προβολής που θα φτιάξουμε είναι 10.


- **Αριθμός D**<br>
![image](https://github.com/Vaggelisvl/Project_2023_1/assets/139377786/2cc12d87-7966-478c-80a8-e779ee20b803)

Στα παραπάνω διαγράμματα βλέπουμε το ποσοστό επιτυχίας της άσκησης αλλάζοντας το D(αριθμός σημείων ανά φύλλο του δέντρου). Συγκεκριμένα στο αριστερό διάγραμμα βλέπουμε το ποσοστο επιλέγοντας το D να είναι 1,2 και 3 έχοντας σταθερό τον αριθμό των κοντινότερων γειτόνων(κ=5). Στο δεξί διάγραμμα επιλέγουμε το D να είναι 3,5 και 7 έχοντας σταθερό το κ σε 10. Παρατηρήσαμε οτι η πιο αποδοτική επιλογή για τον αριθμό D είναι το 3.

















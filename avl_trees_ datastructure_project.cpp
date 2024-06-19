#include <stdio.h>
#include <stdlib.h>

typedef struct DataStructure {          /* THE DATA STRUCTURE */
    struct Node* time_root;              /*ROOT TO TIME AVL TREE*/
    struct Node* quality_root;          /*ROOT TO QUALITY AVL TREE*/
    int s;
    int sCounter;                       /*COUNTER FOR SPECIAL QUALITY*/
    int temp_quality;                   /*TEMPORARY QUALITY VALUE HOLDER*/
    int temp_time;                      /* TEMPORARY TIME VALUE HOLDER */
} DataStructure;

typedef struct Node {                   /*THE NODE OF THE AVL TREES*/
    int time;
    int quality;
    int height;
    int rank;
    int route;
    struct Node* left;
    struct Node* right;
    struct Node* min_quality_ptr;       /*POINTER TO THE NODE WITH THE MINIMUM QUALITY VALUE IN THE NODE'S SUBTREE*/
} Node;

DataStructure Init(int special);                            /*DATA STRUCTURE INITIATOR */
void AddProduct(DataStructure* ds, int time, int quality);      /*ADD PRODUCT TO THE DATA STRUCTURE*/
void RemoveProduct(DataStructure* ds, int time);    /*REMOVE A PRODUCT FROM THE DATA STRUCTURE*/
void RemoveQuality(DataStructure* ds, int quality);     /*REMOVE ALL PRODUCTS WITH THE QUALITY "QUALITY" FROM THE DATA STRUCTURE*/
int GetIthRankProduct(DataStructure ds, int i);             /*RETURN THE ITH LARGEST QUALITY'S TIME*/
int GetIthRankProductBetween(DataStructure ds, int time1, int time2, int i);        /*RETURN THE ITH LARGEST QUALITY'S TIME THAT IS BETWEEN T1 AND T2*/
int Exists(DataStructure ds);           /*RETURN 1 IF NODE WITH QUALITY EQUAL THE S EXISTS, ELSE RETURN 0*/
int searchIth(struct Node* node, int i, int* sum);      /* USED IN GetIthRankProduct, SEACHES AND RETURN THE ITH LARGEST QUALITY'S TIME*/
struct Node* newNode(int time, int quality);           /*CREATE A NEW NODE AND INITIALIZE ITS VALUES*/
struct Node* insertNodeT1(DataStructure* ds, struct Node* node, int time, int quality);     /* INSERT A PRODUCT TO THE TIME AVL TREE */
struct Node* insertNodeT2(DataStructure* ds, struct Node* node, int time, int quality);     /*INSERT A PRODUCT TO THE QUALITY AVL TREE*/
struct Node* leftRotate(struct Node* node, int is_time_tree);       /* LEFT ROTATE FUNCTION*/
struct Node* rightRotate(struct Node* node, int is_time_tree);      /*RIGHT ROTATE FUNCTION*/
struct Node* deleteNodeT1(struct DataStructure* ds, struct Node* node, int time, int deleteFlag);       /*DELETE A NODE FROM TIME AVL TREE*/
struct Node* deleteNodeT2(struct DataStructure* ds, struct Node* node, int quality, int time, int deleteFlag);      /*DELETE A NODE FROM QUALITY AVL TREE*/
struct Node* deleteNodeT2ByQ(struct DataStructure* ds, struct Node* node, int quality, int deleteFlag, int* removeQualityFlag);     /* DELETE NODES FROM QUALITY AVL TREE WITHOUT CARING ABOUT THER TIME*/
struct Node* Balance(struct Node* node, int is_time_tree);          /*BALANCE NODE FUNCTION*/
struct Node* minValueNode(struct Node* node);           /*RETURN THE MIN VALUE NODE IN THE SUBTREE*/
void setMinQuality(struct Node* node);          /*SETS MIN QUALITY POINTER FROM THE ROOT TO A NODE (A NODE THAT ITS QUALITY WAS CHANGED)*/
int getBalance(struct Node* node);              /*RETURNS THE BALANCE DIFFERENCES OF THE NODE*/
int height(struct Node* node);                  /*RETURNS THE HEIGHT OF THE NODE*/
int maxNum(int x, int y);                       /*RETURNS THE MAX VALUE BETWEEN 2 INTIGERS*/
int getMaxQuality(Node* node);                  /*RETURNS THE MAX QUALITY*/
int rank(struct Node* node);                    /*RETURNS THE NODEWS RANK*/
Node* GetMinRankProductBetween(Node* root, Node* t1, Node* t2, Node* LCA, int time1, int time2);       /*RETURNS THE NODE WITH THE MINIMUM QUALITY VALUE IN A SUBTREE*/
Node* findt(Node* root, int time1, int change_flag, int flag);      /*FIND A NODE BY TIME*/
Node* getLCArightMin(Node* node, int time);             /*RETURNS THE NODE WITH THE MINIMAL QUALITY IN THE LCA'S RIGHT SUBTREE*/
Node* getLCAleftMin(Node* node, int time);              /*RETURNS THE NODE WITH THE MINIMAL QUALITY IN THE LCA'S LEFT SUBTREE*/
void getLCA(Node* root, int time2, Node** LCA);         /*FINDS THE LCA OF T1 AND T2*/
void setMinQualityInRoute(Node* root, int time);        /*SETS MIN QUALITY POINTER FROM THE ROOT TO A NODE (A NODE THAT ITS QUALITY WAS CHANGED)*/
int getNumOfNodesInRangeRight(Node* LCA, Node* t);      /*CALCULATES THE NUMBER OF NODES IN RANGE, THAT ARE ON THE RIGHT SIDE OF THE LCA*/
int getNumOfNodesInRangeLeft(Node* LCA, Node* t);       /*CALCULATES THE NUMBER OF NODES IN RANGE, THAT ARE ON THE LEFT SIDE OF THE LCA*/

/*DATA STRUCTURE INITIATOR */
/* RECEIVES AN INT, A SPECIAL QUALITY, AND RETURNS A DATASTRUCTURE BY COPY */
/* TIME COMPLEXITY - O(1) */
/* SPACE COMPLEXITY - O(1) */
DataStructure Init(int special)
{
    struct DataStructure* ds = (struct DataStructure*)malloc(sizeof(struct DataStructure));
    ds->s = special;
    ds->temp_time = 0;
    ds->temp_quality = 0;
    ds->sCounter = 0;
    ds->time_root = NULL;
    ds->quality_root = NULL;
    return *ds;
}

/*ADD PRODUCT */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
void AddProduct(DataStructure* ds, int time, int quality)           /*ADD PRODUCT TO THE DATA STRUCTURE*/
{
    if (ds->time_root == NULL) {                                    /*CHECK IF THE TREES ARE EMPTY*/
        ds->time_root = newNode(time, quality);                     /*THEN CREATE THE ROOT OF EACH TREE*/
        ds->quality_root = newNode(time, quality);
    }
    else
    {
        insertNodeT1(ds, ds->time_root, time, quality);             /*IF TREES EXISTS THEN INSERT A NEW NODE*/
        insertNodeT2(ds, ds->quality_root, time, quality);
    }
    if (quality == ds->s)                                           /* IF THE NEW QUALITY IS EQUAL TO S THEN ADD 1 TO THE SPECIAL QUALITY COUNTER*/
    {
        ds->sCounter++;
    }
}

/*REMOVE PRODUCT*/
/* RECEIVES TIME, FINDS A PRODUCT WITH THIS TIME, AND DELETES IT.IF THERE IS NO PRODUCT WITH THIS TIME, IT DOES NOTHING*/
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
void RemoveProduct(DataStructure* ds, int time)
{
    int deleteFlag = 0;
    deleteNodeT1(ds, ds->time_root, time, deleteFlag);              /*DELETE THE PRODUCT FROM THE TIME TREE (T1 IS TIME TREE)*/
    deleteFlag = 0;
    if (ds->temp_quality == ds->s)                                  /*IF THE DELETED PRODUCT QUALITY WAS EQUAL TO S THEN LOWER THE S COUNTER BY 1 */
    {
        ds->sCounter--;
    }
    deleteNodeT2(ds, ds->quality_root, ds->temp_quality, ds->temp_time, 0);     /* DELETE THE PRODUCT FROM THE QUALITY TREE(T2 IS THE QUALITY TREE) */
    deleteFlag = 0;
    ds->temp_quality = 0;
    ds->temp_time = 0;
}
/*REMOVE PRODUCT*/
/* DELETE ALL THE PRODUCTS WITH THE QUALITY "QUALITY" FROM THE DATA STRUCTURE */
/*TIME COMPLEXITY - O(K*LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
void RemoveQuality(DataStructure* ds, int quality)
{
    int deleteFlag = 0;
    int removeQualityFlag = 0;
    while (removeQualityFlag == 0)                                  /* IF NO MORE PRODUCTS TO DELETE THEN removeQualityFlag WILL BE 1 */
    {
        deleteNodeT2ByQ(ds, ds->quality_root, quality, deleteFlag, &removeQualityFlag);        /*DELETE A PRODUCT BY QUALITY FROM THE DATA STRUCTURE WITH NOT CARING ABOUT ITS TIME VALUE */
        deleteFlag = 0;
        if (ds->temp_quality == ds->s && removeQualityFlag == 0)       /*IF THE DELETED PRODUCT QUALITY WAS EQUAL TO S THEN LOWER THE S COUNTER BY 1*/
        {
            {
                ds->sCounter--;
            }
            deleteNodeT1(ds, ds->time_root, ds->temp_time, deleteFlag);    /*DELETE THE DELETED QUALITY PRODUCT FROM THE TIME TREE*/
        }
    }
}
/*GET ITH RANK PRODUCT*/
/* RETURN THE ITH LARGEST PRODUCT (BY QUALITY) TIME */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
int GetIthRankProduct(DataStructure ds, int i)
{
    int numOfNodes, sum, IthTime;
    if (!ds.time_root) return -1;
    numOfNodes = ds.quality_root->rank;
    if (numOfNodes < i) return -1;
    sum = 0;
    IthTime = searchIth(ds.quality_root, i, &sum);      /*CALL FOR FUNCTION THAT FINDS THE ITH LARGEST PRODUCT AND RETURNS ITS TIME*/
    return IthTime;
}

/*GET ITH RANK PRODUCT BETWEEN*/
/* RETURN THE ITH LARGEST PRODUCT BETWEEN T1 AND T2 TIME */
/*TIME COMPLEXITY - O(I*LOG(N))*/
/*SPACE COMPLEXITY - O(I) */
int GetIthRankProductBetween(DataStructure ds, int time1, int time2, int i)
{
    int maxq, time, numOfNodesInRange, j;
    Node* t1, * t2, * LCA;
    Node** changedNodes_arr;
    int* changedNodesValues_arr;
    if (!ds.time_root) return -1; /*CASE TREES ARE EMPTY */
    maxq = getMaxQuality(ds.quality_root) + 1;
    time = -1;
    t1 = findt(ds.time_root, time1, 1, 1);
    t2 = findt(ds.time_root, time2, 0, 0);
    LCA = NULL;
    getLCA(ds.time_root, time2, &LCA);
    /* WE CHECK IF THE I-QUALITY WERE LOOKING FOR IS BIGGER THAN THE NUMBER OF NODES IN THE RANGE WHICH IS ILLIGAL */
    numOfNodesInRange = 1 + getNumOfNodesInRangeLeft(LCA->left, t1) + getNumOfNodesInRangeRight(LCA->right, t2);
    if (i > numOfNodesInRange) return -1;
    if (t1 == t2) return t1->time;
    changedNodes_arr = (Node**)malloc(sizeof(Node*) * i); /* ARR TO STORE ALL THE NODES THAT ARE MINIMUM OF SUBTREES */
    changedNodesValues_arr = (int*)malloc(sizeof(int) * i); /* ARR TO STORE ALL THE QUALITIES OF NODES THAT ARE MINIMUM OF SUBTREES */

    for (j = 0; j < i; j++)
    {
        Node* minRankNode = GetMinRankProductBetween(ds.time_root, t1, t2, LCA, time1, time2); /* FIND THE NODE WITH THE SMALLEST QUALITY IN RANGE*/
        time = minRankNode->time;
        changedNodes_arr[j] = minRankNode;
        changedNodesValues_arr[j] = minRankNode->quality;
        minRankNode->quality = maxq; /* TEMPORARY INCREASE NODES QUALITY VALUE TO BE BIGGER THAN ALL QUALITIES IN RANGE SO IT WONT BE THE MIN IN RANGE AGAIN */
        setMinQualityInRoute(ds.time_root, time); /* REASSIGN MIN VALUE FOR ALL NODES THAT THEIR MIN QUALITY POINTER POINTED TO minRankNode */
    }
    for (j = 0; j < i; j++)
    {
        changedNodes_arr[j]->quality = changedNodesValues_arr[j]; /* RESTORE ORIGINAL QUALITY VALUE FOR ALL THE NODES WE CHANGED */
        setMinQualityInRoute(ds.time_root, changedNodesValues_arr[j]);
    }
    findt(ds.time_root, time1, 0, 0); /* INITIALIZE ALL FLAGS */
    free(changedNodes_arr);
    free(changedNodesValues_arr);

    return time;
}

/* CALCULATES THE NUMBER OF NODES IN RANGE, THAT ARE ON THE RIGHT SIDE OF THE LCA */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
int getNumOfNodesInRangeRight(Node* LCA, Node* t)
{
    if (!LCA)
        return 0;
    if (LCA == t)
    {
        if (LCA->left)
            return 1 + LCA->left->rank;
    }
    if (LCA->time < t->time)
    {
        int sum = 1;
        if (LCA->left)
            sum += LCA->left->rank;
        if (LCA->right)
            sum += getNumOfNodesInRangeRight(LCA->right, t);
        return sum;
    }
    if (!LCA->left) return 1;
    return getNumOfNodesInRangeRight(LCA->left, t);
}

/* CALCULATES THE NUMBER OF NODES IN RANGE, THAT ARE ON THE LEFT SIDE OF THE LCA */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
int getNumOfNodesInRangeLeft(Node* LCA, Node* t)
{
    if (!LCA)
        return 0;
    if (LCA == t)
    {
        if (LCA->right)
            return 1 + LCA->right->rank;
    }
    if (LCA->time > t->time)
    {
        int sum = 1;
        if (LCA->right)
            sum += LCA->right->rank;
        if (LCA->left)
            sum += getNumOfNodesInRangeLeft(LCA->left, t);
        return sum;
    }
    if (!LCA->right) return 1;
    return getNumOfNodesInRangeLeft(LCA->right, t);
}

/* RETURNS THE NODE WITH THE SMALLEST QUALITY IN RANGE */
/*TIME COMPLEXITY - O(LOG(N)) -- ONLY CALLS 2 FUNCTIONS THAT TAKE LOG(N) TIME EACH*/
/*SPACE COMPLEXITY - O(1) */
Node* GetMinRankProductBetween(Node* root, Node* t1, Node* t2, Node* LCA, int time1, int time2)
{
    Node* t1Min = getLCAleftMin(LCA->left, time1); /* GET THE NODE WITH THE MINIMAL QUALITY IN THE LCA'S LEFT SUBTREE */
    Node* t2Min = getLCArightMin(LCA->right, time2);/* GET THE NODE WITH THE MINIMAL QUALITY IN THE LCA'S RIGHT SUBTREE*/
    /* COMPARE RIGHT & LEFT MIN QUALITIES WITH THE LCA AND RETURN THE MINIMAL ONE, IF 2 QUALITIES ARE THE SAME  THEN TAKE THE ONE WITH THE SMALLER TIME */
    if (t1Min->quality <= t2Min->quality && t1Min->quality <= LCA->quality)
        return t1Min;
    if (LCA->quality < t1Min->quality && LCA->quality <= t2Min->quality)
        return LCA;
    return t2Min;
}

/* FIND A NODE BY TIME, ALSO USES TO INITIALIZE ALL FLAGS THAT WERE CHANGED TO 1 AFTER SEARCHING FOR A NODE */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
Node* findt(Node* root, int time1, int change_flag, int flag)
{
    if (!root)
        return NULL;
    if (change_flag)
        root->route = flag;
    if (root->time < time1)
    {
        if (root->right)
            root = findt(root->right, time1, change_flag, flag);
    }
    else if (root->time > time1)
    {
        if (root->left)
            root = findt(root->left, time1, change_flag, flag);
        else
            return root;
    }
    else return root;
}

/*FINDS THE LCA OF T1 AND T2 */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
void getLCA(Node* root, int time2, Node** LCA)
{
    if (root->time > time2)
        getLCA(root->left, time2, LCA);
    else if (root->time < time2)
        getLCA(root->right, time2, LCA);
    if (root->route && !*LCA) *LCA = root;
}

/* SETS MIN QUALITY POINTER FROM THE ROOT TO A NODE (A NODE THAT ITS QUALITY WAS CHANGED) */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
void setMinQualityInRoute(Node* root, int time)
{
    /* RECURSION GETS TO THE NODE THAT WAS CHANGED AND UPDATES ITS MIN VALUE, THEN FIXES ALL THE NODES UP TO THE ROOT */
    if (!root)
        return;
    if (root->time < time)
    {
        setMinQualityInRoute(root->right, time);
    }
    if (root->time > time)
    {
        setMinQualityInRoute(root->left, time);
    }
    if (root->min_quality_ptr->time == time)
        setMinQuality(root); /* FIX THE MIN QUALITY */
}

/*RETURNS THE NODE WITH THE MINIMAL QUALITY IN THE LCA'S RIGHT SUBTREE */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
Node* getLCArightMin(Node* node, int time)
{
    if (!node)
        return NULL;
    if (node->time == time) /* ON THE RIGHT SIDE, IF WE REACHED T2, ALL NODES IN ITS LEFT SUBTREE ARE VALID */
    {
        if (node->left && node->left->min_quality_ptr->quality <= node->quality)
            return node->left->min_quality_ptr;
        return node;
    }
    if (node->time < time)  /* IF THE NODES TIME IS SMALLER THAN T2, WE NEED TO SEARCH BOTH RIGHT AND LEFT SUBTREES */
    {
        Node* LeftMin = NULL, * RightMin = NULL;
        if (node->left)
            LeftMin = node->left->min_quality_ptr;
        if (node->right)
            RightMin = getLCArightMin(node->right, time);
        if (!LeftMin && !RightMin)
            return node;
        if (LeftMin && !RightMin)
        {
            if (LeftMin->quality <= node->quality)
                return LeftMin;
            return node;
        }
        if (!LeftMin && RightMin)
        {
            if (RightMin->quality < node->quality)
                return RightMin;
            return node;
        }
        /* COMPARE RIGHT & LEFT MIN QUALITIES WITH THE NODE AND RETURN THE MINIMAL ONE, IF 2 QUALITIES ARE THE SAME THEN TAKE THE ONE WITH THE SMALLER TIME */
        if (LeftMin->quality <= RightMin->quality && LeftMin->quality <= node->quality)
            return LeftMin;
        if (node->quality < LeftMin->quality && node->quality <= RightMin->quality)
            return node;
        return RightMin;


    }
    /* CODE REACHES HERE ONLY IF NODES TIME IS BIGGER THAN T2, SO WE NEED TO CHECK ONLY NODES LEFT SUBTREE */
    return getLCArightMin(node->left, time);
}

/*RETURNS THE NODE WITH THE MINIMAL QUALITY IN THE LCA'S LEFT SUBTREE */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
Node* getLCAleftMin(Node* node, int time)
{
    if (!node)
        return NULL;
    if (node->time == time) /* ON THE LEFT SIDE, IF WE REACHED T1, ALL NODES IN ITS RIGHT SUBTREE ARE VALID */
    {
        if (node->right && node->right->min_quality_ptr->quality < node->quality)
            return node->right->min_quality_ptr;
        return node;
    }
    if (node->time > time)  /* IF THE NODES TIME IS BOGGER THAN T1, WE NEED TO SEARCH BOTH RIGHT AND LEFT SUBTREES */
    {
        Node* LeftMin = NULL, * RightMin = NULL;
        if (node->right)
            RightMin = node->right->min_quality_ptr;
        if (node->left)
            LeftMin = getLCAleftMin(node->left, time);
        if (!LeftMin && !RightMin)
            return node;
        if (LeftMin && !RightMin)
        {
            if (LeftMin->quality <= node->quality)
                return LeftMin;
            return node;
        }
        if (!LeftMin && RightMin)
        {
            if (RightMin->quality < node->quality)
                return RightMin;
            return node;
        }
        /* COMPARE RIGHT & LEFT QUALITIES WITH THE NODE AND RETURN THE MINIMAL ONE, IF 2 QUALITIES ARE THE SAME THEN TAKE THE ONE WITH THE SMALLER TIME */
        if (LeftMin->quality <= RightMin->quality && LeftMin->quality <= node->quality)
            return LeftMin;
        if (node->quality < LeftMin->quality && node->quality <= RightMin->quality)
            return node;
        return RightMin;


    }
    /* CODE REACHS HERE ONLY IF NODES TIME IS SMALLER THAN T1, SO WE NEED TO CHECKONLY NODES RIGHT SUBTREE */
    return getLCAleftMin(node->right, time);
}

/*Exists*/
/* RETURN 1 IS NODE WITH QUALITY S EXISTS, ELSE RETURN 0 */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1) */
int Exists(DataStructure ds)
{
    if (ds.sCounter)
    {
        return 1;
    }
    return 0;
}

/* SEARCH THE ITH LARGEST QUALITY NODE IN THE QUALITY TREE AND RETURN ITS TIME */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1) */
int searchIth(struct Node* node, int i, int* sum)
{
    if (node == NULL)
        return -1;
    if (rank(node->left) + *sum + 1 > i)
    {
        return searchIth(node->left, i, sum);
    }
    else if (rank(node->left) + *sum + 1 < i)
    {
        *sum += rank(node->left) + 1;
        return searchIth(node->right, i, sum);
    }
    else if (rank(node->left) + *sum + 1 == i)
    {
        return node->time;
    }
}

/* RETURNS A NEW NODE AND INITIALIZE ALL ITS VALUES */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* newNode(int time, int quality)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->time = time;
    node->quality = quality;
    node->height = 0;
    node->rank = 1;
    node->right = NULL;
    node->left = NULL;
    node->min_quality_ptr = node;
    node->route = 0;
    return node;
}

/*INSERT FUNCTION TO TIME AVL TREE */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* insertNodeT1(DataStructure* ds, struct Node* node, int time, int quality)
{
    int nodeIsRoot = 0;
    if (node == NULL)
    {
        node = newNode(time, quality);
        return node;
    }
    if (time < node->time)
    {
        node->left = insertNodeT1(ds, node->left, time, quality);
    }
    else if (time > node->time)
    {
        node->right = insertNodeT1(ds, node->right, time, quality);
    }
    node->height = maxNum(height(node->left), height(node->right)) + 1;         /* UPDATE THE NODE'S HEIGHT */

    if (ds->time_root == node)
        nodeIsRoot = 1;
    node = Balance(node, 1);                                                    /* BALANCE THE NODE */
    if (nodeIsRoot)
        ds->time_root = node;
    node->rank = rank(node->left) + rank(node->right) + 1;                      /* UPDATE THE NODE'S RANK */
    return node;
}

/*INSERT FUNCTION TO QUALITY AVL TREE */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* insertNodeT2(DataStructure* ds, struct Node* node, int time, int quality)
{
    int nodeIsRoot = 0;
    if (node == NULL)
    {
        node = newNode(time, quality);
        return node;
    }
    if (quality < node->quality)
    {
        node->left = insertNodeT2(ds, node->left, time, quality);
    }
    else if (quality > node->quality)
    {
        node->right = insertNodeT2(ds, node->right, time, quality);
    }
    else if (quality == node->quality)                                      /* CASE WE REACH A NODE WITH THE SAME QUALITY WE INSERT THE NEW NODE BY ITS TIME */
    {
        if (time < node->time)
        {
            node->left = insertNodeT2(ds, node->left, time, quality);
        }
        else if (time > node->time)
        {
            node->right = insertNodeT2(ds, node->right, time, quality);
        }
    }
    node->height = maxNum(height(node->left), height(node->right)) + 1;         /*UPDATE THE NODE'S HEIGHT */
    if (ds->quality_root == node)
        nodeIsRoot = 1;
    node = Balance(node, 0);                                                     /* BALANCE THE NODE */
    if (nodeIsRoot)
        ds->quality_root = node;
    node->rank = rank(node->left) + rank(node->right) + 1;                      /* UPDATE THE NODE'S RANK */
    return node;
}

/*LEFT ROTATE FUNCTION */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* leftRotate(struct Node* node, int is_time_tree)
{
    struct Node* temp1 = node->right;
    struct Node* temp2 = temp1->left;
    temp1->left = node;
    node->right = temp2;
    node->height = maxNum(height(node->left), height(node->right)) + 1;
    temp1->height = maxNum(height(temp1->left), height(temp1->right)) + 1;
    node->rank = 1 + rank(node->left) + rank(node->right);
    temp1->rank = 1 + rank(node->left) + rank(node->right);
    if (is_time_tree)
    {
        setMinQuality(temp1);                           /* CASE TIME TREE THEN UPDATE THE MINIMUM QUALITY POINTER IN THE SUBTREE */
        if (temp2) setMinQuality(temp2);
        setMinQuality(node);
    }
    return temp1;
}

/*RIGHT ROTATE FUNCTION */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* rightRotate(struct Node* node, int is_time_tree)
{
    struct Node* temp1 = node->left;
    struct Node* temp2 = temp1->right;
    temp1->right = node;
    node->left = temp2;
    node->height = maxNum(height(node->left), height(node->right)) + 1;
    temp1->height = maxNum(height(temp1->left), height(temp1->right)) + 1;
    node->rank = 1 + rank(node->left) + rank(node->right);
    temp1->rank = 1 + rank(node->left) + rank(node->right);
    if (is_time_tree)
    {
        setMinQuality(temp1);                               /* CASE TIME TREE THEN UPDATE THE MINIMUM QUALITY POINTER OF THE SUBTREE */
        if (temp2) setMinQuality(temp2);
        setMinQuality(node);
    }
    return temp1;
}

/* SET THE MINIMUM QUALITY POINTER IN THE SUBTREE */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
void setMinQuality(struct Node* node)
{
    node->min_quality_ptr = node;
    if (node->left)
    {
        Node* minQleft = node->left->min_quality_ptr;              /* HOLDS THE MINIMUM POINTER OF THE LEFT SUBTREE */
        if (minQleft->quality <= node->min_quality_ptr->quality)
            node->min_quality_ptr = minQleft;                        /* UPDATE THE NODES MIN POINTER IF NEEDED */
    }
    if (node->right)
    {
        Node* minQright = node->right->min_quality_ptr;             /* HOLDS THE MINIMUM POINTER OF THE RIGHT SUBTREE */
        if (minQright->quality <= node->min_quality_ptr->quality)
            node->min_quality_ptr = minQright;                       /*UPDATE THE NODES MIN POINTER IF NEEDED */
    }

}

/* DELETE A PRODUCT FROM TIME AVL TREE (T1) */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* deleteNodeT1(struct DataStructure* ds, struct Node* node, int time, int deleteFlag)
{
    int nodeIsRoot = 0;
    if (node == NULL)
    {
        return node;
    }
    if (time < node->time)
    {
        node->left = deleteNodeT1(ds, node->left, time, deleteFlag);
    }
    else if (time > node->time)
    {
        node->right = deleteNodeT1(ds, node->right, time, deleteFlag);
    }
    else
    {
        if (!(deleteFlag))
        {
            ds->temp_quality = node->quality;
            ds->temp_time = node->time;
            deleteFlag = 1;
        }
        if ((node->left == NULL) || (node->right == NULL))
        {
            struct Node* temp = node->left ? node->left : node->right;
            if (temp == NULL)
            {
                temp = node;
                free(node);
                return NULL;
            }
            else
            {

                if (node->right)
                {
                    temp = node->right;
                }
                else
                {
                    temp = node->left;
                }
                free(node);
                temp->rank = rank(temp->left) + rank(temp->right) + 1;  /* UPDATE THE NODE'S RANK */
                return temp;
            }
        }
        else            /* REPLACE THE DELETED NODE WITH THE MIN NODE IN ITS RIGHT SUBTREE */
        {
            struct Node* temp = minValueNode(node->right);
            node->time = temp->time;
            node->quality = temp->quality;
            node->right = deleteNodeT1(ds, node->right, temp->time, deleteFlag);
        }
    }
    node->height = maxNum(height(node->left), height(node->right)) + 1;     /* UPDATE THE NODE'S HEIGHT */
    if (ds->time_root == node)
        nodeIsRoot = 1;
    node = Balance(node, 1);                    /* BALANCE THE NODE */
    if (nodeIsRoot)
        ds->time_root = node;
    node->rank = rank(node->left) + rank(node->right) + 1;      /* UPDATE THE NODE'S RANK */
    return node;
}

/* DELETE A PRODUCT FROM THE QUALITY AVL TREE */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* deleteNodeT2(struct DataStructure* ds, struct Node* node, int quality, int time, int deleteFlag)
{
    int nodeIsRoot = 0;
    if (node == NULL)
    {
        return node;
    }
    if (quality < node->quality)
    {
        node->left = deleteNodeT2(ds, node->left, quality, time, deleteFlag);
    }
    else if (quality > node->quality)
    {
        node->right = deleteNodeT2(ds, node->right, quality, time, deleteFlag);
    }
    else if ((quality == node->quality) && (time < node->time))                     /* CASE NODE HAS THE SAME QUALITY AS THE ONE WE WANT TO DELETE THEN SEARCH BY TIME */
    {
        node->left = deleteNodeT2(ds, node->left, quality, time, deleteFlag);
    }
    else if ((quality == node->quality) && (time > node->time))
    {
        node->right = deleteNodeT2(ds, node->right, quality, time, deleteFlag);
    }
    else
    {
        if (!(deleteFlag))
        {
            ds->temp_quality = node->quality;
            ds->temp_time = node->time;
            deleteFlag = 1;
        }
        if ((node->left == NULL) || (node->right == NULL))
        {
            struct Node* temp = node->left ? node->left : node->right;
            if (temp == NULL)
            {
                temp = node;
                free(node);
                return NULL;
            }
            else
            {
                if (node->right)
                {
                    temp = node->right;
                }
                else
                {
                    temp = node->left;
                }
                free(node);
                temp->rank = rank(temp->left) + rank(temp->right) + 1;      /* UPDATE NODE'S RANK */
                return temp;
            }
        }
        else                      /* REPLACE THE DELETED NODE WITH THE MIN NODE IN ITS RIGHT SUBTREE */
        {
            struct Node* temp = minValueNode(node->right);
            node->time = temp->time;
            node->quality = temp->quality;
            node->right = deleteNodeT2(ds, node->right, temp->quality, temp->time, deleteFlag);
        }
    }
    node->height = maxNum(height(node->left), height(node->right)) + 1;         /*  UPDATE NODE'S HEIGHT */
    if (ds->quality_root == node)
        nodeIsRoot = 1;
    node = Balance(node, 0);                    /* BALANCE NODE */
    if (nodeIsRoot)
        ds->quality_root = node;
    node->rank = rank(node->left) + rank(node->right) + 1;          /* UPDATE NODE'S RANK */
    return node;
}

/* DELETE ALL QUALITIES FROM QUALITY AVL TREE WITHOUT CARING ABOUT THEIR TIME */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* deleteNodeT2ByQ(struct DataStructure* ds, struct Node* node, int quality, int deleteFlag, int* removeQualityFlag)
{
    int nodeIsRoot = 0;
    if (node == NULL)
    {
        *removeQualityFlag = 1;
        return node;
    }
    if (quality < node->quality)
    {
        node->left = deleteNodeT2ByQ(ds, node->left, quality, deleteFlag, removeQualityFlag);
    }
    else if (quality > node->quality)
    {
        node->right = deleteNodeT2ByQ(ds, node->right, quality, deleteFlag, removeQualityFlag);
    }
    else
    {
        if (!(deleteFlag))
        {
            ds->temp_quality = node->quality;
            ds->temp_time = node->time;
            deleteFlag = 1;
        }
        if ((node->left == NULL) || (node->right == NULL))
        {
            struct Node* temp = node->left ? node->left : node->right;
            if (temp == NULL)
            {
                temp = node;
                free(node);
                return NULL;
            }
            else
            {
                if (node->right)
                {
                    temp = node->right;
                }
                else
                {
                    temp = node->left;
                }
                free(node);
                temp->rank = rank(temp->left) + rank(temp->right) + 1;      /* UPDATE NODE'S RANK */
                return temp;
            }
        }
        else                          /* REPLACE THE DELETED NODE WITH THE MIN NODE IN ITS RIGHT SUBTREE */
        {
            struct Node* temp = minValueNode(node->right);
            node->time = temp->time;
            node->quality = temp->quality;
            node->right = deleteNodeT2(ds, node->right, temp->quality, temp->time, deleteFlag);
        }
    }
    node->height = maxNum(height(node->left), height(node->right)) + 1;     /* UPDATE NODE'S HEIGHT */
    if (ds->quality_root == node)
        nodeIsRoot = 1;
    node = Balance(node, 0);                /* BALANCE NODE */
    if (nodeIsRoot)
        ds->quality_root = node;
    node->rank = rank(node->left) + rank(node->right) + 1;              /* UPDATE NODE'S RANK */
    return node;
}

/* BALANCE NODE FUNCTION */
/*TIME COMPLEXITY - O(1))*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* Balance(struct Node* node, int is_time_tree)
{
    struct Node* temp;
    if (getBalance(node) >= -1 && getBalance(node) <= 1)        /* CASE NODE BALANCED */
    {
        if (is_time_tree)                   /* CASE TIME TREE THEN UPDATE THE NODE'S MINIMUM QUALITY POINTER */
            setMinQuality(node);
        return node;
    }
    else if (getBalance(node) > 1)      /* CASE LEFT HEAVY */
    {
        temp = node->left;
        if (getBalance(temp) <= -1)      /* CASE LEFT RIGHT HEAVY */
        {
            node->left = leftRotate(temp, is_time_tree);
        }
        return rightRotate(node, is_time_tree);
    }
    else                                /* CASE RIGHT HEAVY */
    {
        temp = node->right;
        if (getBalance(temp) >= 1)       /* CASE RIGHT LEFT HEAVY */
        {
            node->right = rightRotate(temp, is_time_tree);
        }
        return leftRotate(node, is_time_tree);
    }
}

/* RETURNS THE MINIMUM VALUE NODE IN THE TREE */
/*TIME COMPLEXITY - O(LOG(N))*/
/*SPACE COMPLEXITY - O(1)*/
struct Node* minValueNode(struct Node* node)
{
    struct Node* current = node;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}
/* RETURNS THE BALANCE DIFFERENCES OF THE NODE */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
int getBalance(struct Node* node)
{
    if (node == NULL)
    {
        return 0;
    }
    return height(node->left) - height(node->right);
}

/* RETURNS THE HEIGHT OF THE NODE */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
int height(struct Node* node)
{
    if (node == NULL)
    {
        return -1;
    }
    return node->height;
}

/* RETURNS THE RANK OF THE NODE */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
int rank(struct Node* node)
{
    if (node == NULL)
    {
        return 0;
    }
    return node->rank;
}


/* RETURNS THE MAX VALUE BETWEEN 2 INTEGERS */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
int maxNum(int x, int y)
{
    return (x > y) ? x : y;
}

/* RETURNS THE MAX QUALITY VALUE IN THE TREE */
/*TIME COMPLEXITY - O(1)*/
/*SPACE COMPLEXITY - O(1)*/
int getMaxQuality(Node* node)
{
    if (!node)
        return 0;
    if (node->right == NULL)
        return node->quality;
    getMaxQuality(node->right);
}

/*FUNCTION TO PRINT A SINGLE NODE*/
void printNode(Node* node) {
    if (node == NULL) {
        return;
    }

    printf("Node (time: %d, quality: %d, height: %d, rank: %d, route: %d)\n",
        node->time, node->quality, node->height, node->rank, node->route);

    if (node->min_quality_ptr) {
        printf("  -> Min Quality Ptr: Node (time: %d, quality: %d)\n",
            node->min_quality_ptr->time, node->min_quality_ptr->quality);
    }

    printNode(node->left);
    printNode(node->right);
}

/*FUNCTION TO PRINT THE ENTIRE DATA STRUCTURE*/
void printDataStructure(DataStructure* ds) {
    if (ds == NULL) {
        printf("DataStructure is NULL\n");
        return;
    }

    printf("DataStructure:\n");
    printf("s: %d\n", ds->s);
    printf("sCounter: %d\n", ds->sCounter);
    printf("temp_quality: %d\n", ds->temp_quality);
    printf("temp_time: %d\n", ds->temp_time);

    printf("\nTime AVL Tree:\n");
    if (ds->time_root) {
        printNode(ds->time_root);
    }
    else {
        printf("Time AVL Tree is empty\n");
    }

    printf("\nQuality AVL Tree:\n");
    if (ds->quality_root) {
        printNode(ds->quality_root);
    }
    else {
        printf("Quality AVL Tree is empty\n");
    }
}


int main() {
    DataStructure ds = Init(11); // initializes an empty data structure
    AddProduct(&ds, 4, 11); // Adds a product at time t=4 and quality q=11
    AddProduct(&ds, 6, 12); // Adds a product at time t=6 and quality q=12
    AddProduct(&ds, 2, 13); // Adds a product at time t=2 and quality q=13
    AddProduct(&ds, 1, 14); // Adds a product at time t=1 and quality q=14
    AddProduct(&ds, 3, 15); // Adds a product at time t=3 and quality q=15
    AddProduct(&ds, 5, 17); // Adds a product at time t=5 and quality q=17
    AddProduct(&ds, 7, 17); // Adds a product at time t=7 and quality q=17
    GetIthRankProduct(ds, 1); //The i=1 best product has time t=4 and quality q=11,returns 4
    GetIthRankProduct(ds, 2); //The i=2 best product has time t=6 and quality q=12,returns 6
    GetIthRankProduct(ds, 6); //The i=”6 best product” has time t=5 and quality q=17,returns 5
    GetIthRankProduct(ds, 7); //The i=”7 best product” has time t=7 and quality q=17,returns 7
    GetIthRankProductBetween(ds, 2, 6, 3); // looks at values with time {2,3,4,5,6} and returns the i = ”3 best product” between them, which has time t = 2.
    Exists(ds); // returns 1, since there exists a product with quality q=s=11
    RemoveProduct(&ds, 4); // removes product with time t=4 from the data structure
    Exists(ds); // returns 0, since there is no product with quality q=s=11
    printDataStructure(&ds);
}
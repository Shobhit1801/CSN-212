/*
	Shobhit Mittal
	14116061, ECE 3rd Year
	Implementation of Interval Tree (insertion,deletion and search)
	(using BST  )
*/

#include<iostream>
using namespace std;

struct interval
{
	int low;
	int high;
};

struct node
{
	struct interval data;
	int max;
	struct node* left; // pointer to left node
	struct node* right; //pointer to right node
};


// function to create a new node 
struct node* createNode(interval data)
{
	struct node* temp = new node; //if new is not used then temp will get deallocated 
	temp->data=data;
	temp->max=data.high;
	temp->left=NULL;
	temp->right=NULL;
	return temp;	
}

// function to insert data in bst
struct node* insert(struct node* root,interval data)
{
	struct node* temp=root;
	if(temp==NULL) {
		return createNode(data);
	}
	
	if(temp->max<data.high)	{
		temp->max=data.high;
	}

	if(data.low>=(temp->data).low) {
		temp->right=insert(temp->right,data);				
	}
	else {
		temp->left=insert(temp->left,data);
	}
		
	return root;
}

//function for searching an elemnt's address
// assumption: no data is repeated in any node
struct node* search(struct node* root,interval data)
{
	struct node* temp=root;
	while(temp!=NULL && (data.low>temp->data.high || data.high<temp->data.low))
	{
		if(temp->left!=NULL && data.low<temp->left->max) {
			temp=temp->left;
		}
		else {
			temp=temp->right;
		}
	}
	return temp;
}

//used only in deleteKey function. So assumed that data is definitely present in tree
struct node* searchParent(struct node* root,interval data)
{
	struct node* temp=root;
	struct node* par=NULL;
	while(temp!=NULL && (data.low!=temp->data.low || data.high!=temp->data.high))
	{
		par=temp;
		if(temp->left!=NULL && data.low<temp->data.low) {
				temp=temp->left;
		}
		else {
			temp=temp->right;
		}
		
	}
	return par;	
}

//used to update the max value in the tree after node deletion
void update(struct node* root,struct node* last)
{
	//cout<<last->data.low;
	if(root==NULL)	return;
	if(last->data.low < root->data.low)
		update(root->left,last);
	else update(root->right,last);
	
	if(root->left==NULL && root->right==NULL)
		root->max=root->data.high;
	else
	{
		if(root->left!=NULL && root->right!=NULL)
		{
			root->max=max(root->data.high,root->left->max);
			root->max=max(root->max,root->right->max);
		}
		else{
			if(root->left!=NULL)	root->max=max(root->data.high,root->left->max);
			if(root->right!=NULL)	root->max=max(root->data.high,root->right->max);	
		}
		
	}
}

//assumption: data being deleted is present in tree
struct node* deleteKey(struct node* root,interval data)
{
	struct node* par=new node;
	par=searchParent(root,data);
	//cout<<par->data.low<<endl;
	if(par!=NULL)
	{
		struct node* delNode=NULL;	//node to be deleted
		char ch='r';	//relationship between parent and child to be deleted
		if(data.low>=par->data.low)	delNode=par->right;
		else {
			delNode=par->left;
			ch='l';	//delNode is parent's left child
		}
		
		//cout<<delNode->data.low;
		if(delNode->left==NULL && delNode->right==NULL)	//no child: leaf node
		{
			if(ch=='l')	par->left=NULL;
			else par->right=NULL;
	
			update(root,par);
			return root;
		}
		
		if(delNode->left==NULL || delNode->right==NULL)	// only 1 child
		{
			if(delNode->left==NULL)	//left node of delNode is null. So right node will be attached to the parent
			{
				if(ch=='l')	par->left=delNode->right;
				else par->right=delNode->right;
			}
			else	//right node of delNode is null. So left node will be attached to the parent
			{
				if(ch=='l')	par->left=delNode->left;
				else par->right=delNode->left;
			}
			update(root,par);
			return root;
		}
		
		//2 children
		struct node* leftMost=delNode->right;	//stores left most child of right subtree of delNode
		while(leftMost->left!=NULL)
			leftMost=leftMost->left;
		
		leftMost->left=delNode->left;
		if(leftMost!=delNode->right)
		{
			delNode->right->left=leftMost->right;
			leftMost->right=delNode->right;
		}
		if(ch=='l')	par->left=leftMost;
		else par->right=leftMost;
		if(leftMost->right!=NULL)
			update(root,leftMost->right);
		else update(root,leftMost);
		return root;
	}
	
	else
	{
		if(root->left==NULL && root->right==NULL)	
			return NULL;
		if(root->left==NULL || root->right==NULL)	
		{
			if(root->left!=NULL)	return root->left;
			else return root->right;
		}
		
	//	cout<<"yes"<<endl;
		struct node* leftMost=root->right;	//stores left most child of right subtree of delNode
		while(leftMost->left!=NULL)
			leftMost=leftMost->left;
	//	cout<<leftMost->data<<endl;
		leftMost->left=root->left;
		if(leftMost!=root->right)
		{
			root->right->left=leftMost->right;
			leftMost->right=root->right;
		}
		if(leftMost->right!=NULL)
			update(leftMost,leftMost->right);
		else update(leftMost,leftMost);
		return leftMost;
	}
}

void inorder(struct node* root)
{
	if(root==NULL)	return;
	inorder(root->left);
	if(root!=NULL)
	{
		cout<<"low= "<<root->data.low<<" "<<"high= "<<root->data.high<<" "<<"max= "<<root->max<<endl;
		inorder(root->right);
	}
}

/* Compute the "height" of a tree -- the number of
    nodes along the longest path from the root node
    down to the farthest leaf node.*/
int height(struct node* node)
{
    if (node==NULL)
        return 0;
    else
    {
        /* compute the height of each subtree */
        int lheight = height(node->left);
        int rheight = height(node->right);
 
        /* use the larger one */
        if (lheight > rheight)
            return(lheight+1);
        else return(rheight+1);
    }
}

/* Print nodes at a given level */
void printGivenLevel(struct node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        cout<<"low= "<<root->data.low<<" "<<"high= "<<root->data.high<<" "<<"max= "<<root->max<<endl;
    else if (level > 1)
    {
        printGivenLevel(root->left, level-1);
        printGivenLevel(root->right, level-1);
    }
}
 
void printLevelOrder(struct node* root)
{
    int h = height(root);
    int i;
    for (i=1; i<=h; i++)
    {
    	printGivenLevel(root, i);
    	//cout<<endl;
    }
        
}
 
 
int main()
{
	struct node* root=NULL;
	interval data={15,20};
	root = insert(root, data);
    root=insert(root, {10,30});
    root=insert(root, {17,19});
    root=insert(root, {5,20});
    root=insert(root, {12,15});
    root=insert(root, {30,40});
    //root=insert(root, 80);
    struct node* temp;
    temp=search(root,{21,23});
	cout<<"Level wise Traversal:"<<endl;
    printLevelOrder(root);
  //  struct node* temp=search(root,60);
  //  cout<<temp->data.low;
    
   
}



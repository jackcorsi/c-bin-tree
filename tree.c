#include<stdio.h>
#include<stdlib.h>

//Sets whether deleteNode(Node *, int) should print a message if the specified value is not in the tree
#define TREE_OPERATIONS__REPORT_NOT_FOUND 1


struct Node{
	int data;
	struct Node *left;
	struct Node *right;
};
typedef struct Node Node;

void tree_insertNode(Node *root, int value); //Helper functions
Node *tree_removeNode(Node *root);
int tree_removeSmallest(Node *root);


Node* insertNode(Node *root, int value)
{
	//If no tree is provided, create a new one
	if (!root) 
	{
		root = (Node *) malloc(sizeof(Node));
		root->data = value;
		return root;
	}

	tree_insertNode(root, value);
	return root;
}

/*
	Tail recursive helper function assumes that (root != NULL) is guaranteed by the caller, then checks this is true of the appropriate child
	before the recursive call. Returns void as there is no need to pass back pointers for the entire traversal - the returned node
	from insertNode is always the root of the tree
*/
void tree_insertNode(Node *root, int value)
{
	if (value <= root->data)
	{
		if (root->left)
			tree_insertNode(root->left, value);
		else 
		{
			root->left = (Node *) malloc(sizeof(Node));
			root->left->data = value;
		}
	}
	else
	{
		if (root->right)
			tree_insertNode(root->right, value);
		else
		{
			root->right = (Node *) malloc(sizeof(Node));
			root->right->data = value;
		}
	}
}


void freeSubtree(Node *N)
{
	if (N) //If null, do nothing
	{
		freeSubtree(N->left);
		freeSubtree(N->right);
		free(N);	
	}
}

void printSubtree(Node *N)
{
	if (N) 
	{
		printSubtree(N->left);
		printf("%d", N->data);
		printSubtree(N->right);
	}
}

int countNodes(Node *N)
{
	if (!N)
		return 0;

	return 1 + countNodes(N->left) + countNodes(N->right);
}

//deleteNode works recrusively by returning to the caller the value that should be assigned as the caller's left or right child
//this allows the pointer of the parent to be updated when its child has been removed
Node *deleteNode(Node *root, int value)
{
	//Exit if we've reached a null tree - value not found
	if (!root)
	{
		if (TREE_OPERATIONS__REPORT_NOT_FOUND)
			printf("Non-fatal: tree.c deleteNode(Node *, int) specified value (%d) not located or NULL tree given as argument", value);
		return NULL;
	}
	//Reassign child pointer to return value - it will be different if the child has been deleted, or the same if it hasn't
	else if (value < root->data)
	{
		root->left = deleteNode(root->left, value);
		return root;
	}
	else if (value > root->data)
	{
		root->right = deleteNode(root->right, value);
		return root;
	}
	//value == data: we are at the node to be removed
	else
	{
		if (root->right) 
		{
			//If there is a right child with a left subtree, exract the least value from this and replace our value
			if (root->right->left)
			{
				root->data = tree_removeSmallest(root->right);
				return root;
			}
			else //If there is a right child with no left subtree, simply promote this node to our root position
			{
				Node *new_root = root->right;
				new_root->left = root->left;
				free(root);
				return new_root;
			}
		}
		else 
		{
			//If there is no right subtree, delete and replace parent's child with left subtree
			Node *new_root = root->left;
			free(root);
			return new_root;
		}
	}
}

int tree_removeSmallest(Node *root)
{
	if (root->left->left) //root->left is guranteed non-null by the caller
		return tree_removeSmallest(root->left);
	else
	{
		//The left child has no left subtree - it is the smallest node
		//we'll extract its data, delete it and replace it with its right subtree
		//Then pass back the extracted value to the caller
		int value = root->left->data;
		Node *new_child = root->left->right;
		free(root->left);
		root->left = new_child;
		return value;
	}
}

int main()
{
	Node *root=NULL;
	
	
    root=insertNode(root, 14);
	root=insertNode(root, 12);
	root=insertNode(root, 6);
	root=insertNode(root, 17);
	root=insertNode(root, 8);
   	printSubtree(root);
	printf("\n");
	root=deleteNode(root,14);
	root=deleteNode(root,8);
   	printSubtree(root);
	freeSubtree(root);
	root = NULL;
	printf("Number of nodes=%d\n", countNodes(root));
	return 0;

	return 0;
}





#ifndef QTREE_H
#define QTREE_H

struct point {
	int x;
	int y;
};

struct quadtree {
	struct point *key;
	char *value;
	
	struct quadtree *northwest;
	struct quadtree *northeast;
	struct quadtree *southwest;
	struct quadtree *southeast;
};

#endif

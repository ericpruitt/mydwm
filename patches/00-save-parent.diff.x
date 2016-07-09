diff --git a/dwm.c b/dwm.c
index b2bc9bd..be29611 100644
--- a/dwm.c
+++ b/dwm.c
@@ -94,6 +94,7 @@ struct Client {
 	int bw, oldbw;
 	unsigned int tags;
 	int isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
+	Client *parent;
 	Client *next;
 	Client *snext;
 	Monitor *mon;
@@ -660,11 +661,16 @@ createmon(void)
 void
 destroynotify(XEvent *e)
 {
-	Client *c;
+	Client *c, *k;
 	XDestroyWindowEvent *ev = &e->xdestroywindow;
 
-	if ((c = wintoclient(ev->window)))
+	if ((c = wintoclient(ev->window))) {
+		for (m = mons; m; m = m->next)
+			for (k = m->clients; k; k = c->next)
+				if (c->parent == k)
+					c->parent = NULL;
 		unmanage(c, 1);
+	}
 }
 
 void
@@ -1048,7 +1054,8 @@ manage(Window w, XWindowAttributes *wa)
 	c->oldbw = wa->border_width;
 
 	updatetitle(c);
-	if (XGetTransientForHint(dpy, w, &trans) && (t = wintoclient(trans))) {
+	if (XGetTransientForHint(dpy, w, &trans) && (c->parent = t = wintoclient(trans))) {
+		printf("\"%s\" comes from \"%s\"\n", c->name, t->name);
 		c->mon = t->mon;
 		c->tags = t->tags;
 	} else {

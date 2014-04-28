


//calculate destination in steps according to global xyz
void move2globalxyz(float x,float y,float z,float theta,int *ns1,int *ns2,int *ns3,int *sv);
void move2localxyz(float x,float y,float z,int *ns1,int *ns2,int *ns3);

void GeneratePath(int fromLayer,int fromPosition,int toLayer,int toPosition,int firstLayerBlockAlignWithX);//layer number from 0, position number is from 0 to 2 and 0 is the closest to the origin

int parameterCheck(); //return 0 OKAY, return 1 error

void readSettings();
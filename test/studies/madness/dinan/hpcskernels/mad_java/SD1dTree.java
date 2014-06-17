
public class SD1dTree {

    SD1dTree parent;
    SD1dTree[] child;
    int n;
    int l;
    Tensor1d s;
    Tensor1d d;
    
    // set defaults
    private void init(){
        this.parent = null;
        this.child = new SD1dTree[2];
        for(int i = 0; i < 2; i++)
            this.child[i] = null;
        this.n = 0;
        this.l = 0;
        this.s = null;
        this.d = null;
    }
    
    /** Constructor

    */
    public SD1dTree(){
        init();
    }
    
    /** Constructor

    */
    public SD1dTree(int n, int l){
        init();
        this.n = n;
        this.l = l;
    }
    
}


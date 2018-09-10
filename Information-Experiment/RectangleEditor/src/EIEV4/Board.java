package EIEV4;

import java.awt.Color;
import java.util.ArrayList;
import java.util.List;

class Board{
    private final int MAX_RECTANGLE = 10; // Boardが保持できる長方形の最大数 
    private final int INITIAL_WIDTH = 500; // x座標方向の初期値
    private final int INITIAL_HEIGHT = 400; // y座標方向の初期値
    private final Color INITIAL_COLOR = Color.WHITE; //色の初期値

    private List<Rectangle> rectangles; // 実際に長方形を保持する配列.
    private int width; // x方向の大きさ
    private int height; // y方向の大きさ
    private Color color; //色

    public Board(){
    	this.rectangles = new ArrayList<>();
        this.width = this.INITIAL_WIDTH;
        this.height = this.INITIAL_HEIGHT;
        this.color = this.INITIAL_COLOR;
    }

    public Board(int width, int height){
    	this.rectangles = new ArrayList<>();
        this.width = width;
        this.height = height;
        this.color = this.INITIAL_COLOR;
    }

    // 長方形が不変条件を満たすか判定
    private boolean Invariants(Rectangle r){
        if(r.getWidth()<=0 || r.getHeight()<=0) return false; // 長方形の幅か高さが異常ならば失敗
        if(this.rectangles.contains(r)) return false; // 同じ長方形がある場合は失敗
        if(r.isPoint() || r.isLine()) return false; // 長方形が点か線の場合は失敗
        if(!this.isIncludeRectangleInBoard(r)) return false; // 長方形がボードの外にある場合は失敗
        return true;
    }
    
    // 長方形がボード内にあるか判定
    public boolean isIncludeRectangleInBoard(Rectangle r){
        boolean left_wide = r.getLeftXAxis() >= 0;
        boolean right_wide = r.getRightXAxis() <= this.width;
        boolean top_height = r.getTopYAxis() >= 0;
        boolean bottom_height = r.getBottomYAxis() <= this.height;
        return (left_wide && right_wide && top_height && bottom_height);
    }

    // 保持しているすべての長方形を返す
    public List<Rectangle> getRectangles(){
        List<Rectangle> new_recs = new ArrayList<>();
        for(Rectangle r: this.rectangles)
            new_recs.add(new Rectangle(r));
        return new_recs;
    }

    // 背景色を取得
    public Color getColor(){
        return this.color;
    }

    // ボードの幅を返す.
    public int getWidth(){
        return this.width;
    }

    // ボードの高さを返す.
    public int getHeight(){
        return this.height;
    }

    // ボードのサイズを変更する.
    public boolean setSize(int width, int height){
        // 保持している長方形が1つでも新しいサイズをはみ出すならば, この処理は失敗
        for(Rectangle r: this.rectangles)
            if(r.getRightXAxis()>width || r.getBottomYAxis()>height) 
                return false;
        
        this.width = width;
        this.height = height;
        return true;
    }

    public int getRectangleIndex(int x, int y){
        for(int i=this.rectangles.size()-1; i>=0; --i){
            Rectangle r = this.rectangles.get(i);
            double left_x = r.getLeftXAxis();
            double right_x = r.getRightXAxis();
            double top_y = r.getTopYAxis();
            double bottom_y = r.getBottomYAxis();
            if(left_x<x && x<right_x && top_y<y && y<=bottom_y)
                return i;
        }
        return -1;
    }


    // 長方形が追加できる状況であるかを判定する.
    public boolean canAppendRectangle(){
        return (rectangles.size() < MAX_RECTANGLE);
    }

    // indexに対応する長方形が存在するかを判定する.
    public boolean existRectangle(int index){
        return (0 <= index) && (index < rectangles.size());
    }

    // 長方形の個数を返す.
    public int size(){
        return this.rectangles.size();
    }

    // ボードに長方形を追加する.
    public boolean appendRectangle(Rectangle r){
        if(!this.Invariants(r)) return false;
        if(!canAppendRectangle()) return false; // 最大数に到達している場合は失敗
        
        this.rectangles.add(r);
        return true;
    }

    // 指定されたインデックスの長方形を移動する.
    public boolean moveRectangle(int index, double mx, double my) throws IndexOutOfBoundsException{
        Rectangle r = this.rectangles.get(index).getMovedRectangle(mx, my);
        if(!this.Invariants(r)) return false;

        this.rectangles.set(index, r);
        return true;
    }
    
    // 指定されたインデックスの長方形を入れ替える.
    public boolean moveRectangle(int index, Rectangle r) throws IndexOutOfBoundsException{
        if(!this.Invariants(r)) return false;

        this.rectangles.set(index, r);
        return true;
    }

    // 指定されたインデックスの長方形を拡大する.
    public boolean expandRectangle(int index, double mx, double my) throws IndexOutOfBoundsException{
        Rectangle r = this.rectangles.get(index).getExpandRectangle(mx, my);
        if(!this.Invariants(r)) return false;
        
        this.rectangles.set(index, r);
        return true;
    }

    // 指定されたインデックスの長方形を削除する.
    public boolean removeRectangle(int index) throws IndexOutOfBoundsException{
        this.rectangles.remove(index);
        return true;
    }

    // 保持している全ての長方形を削除する.
    public boolean removeAllRectangles(){
        
        this.rectangles.clear();
        return true;
    }

    // 2つの長方形の重なりから新しい長方形を作成する.
    public boolean intersect(int index1, int index2) throws IndexOutOfBoundsException{
        Rectangle r1 = this.rectangles.get(index1);
        Rectangle r2 = this.rectangles.get(index2);
        Rectangle r = r1.getOverlapRetangle(r2);
        if(r == null) return false;
        return this.appendRectangle(r);
    }

    @Override
    public String toString(){
        String str = "";
        for(Rectangle r: this.rectangles){
            str += (
                "rectangle number: " + (this.rectangles.indexOf(r)+1) + "\n" +
                r.toString() + "\n"
            );
        }
        return str;
    }
}
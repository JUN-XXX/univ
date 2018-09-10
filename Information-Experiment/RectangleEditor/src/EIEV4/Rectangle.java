package EIEV4;

import java.util.Arrays;
import java.awt.Color;


class Rectangle{
    private static final Color INITIAL_COLOR = Color.gray;

    public static class CalcColor{

        private static String colorHex(Color c){
            int ri = c.getRed();
            int gi = c.getGreen();
            int bi = c.getBlue();
            String rs = String.format("%02x", Integer.valueOf(ri));
            String gs = String.format("%02x", Integer.valueOf(gi));
            String bs = String.format("%02x", Integer.valueOf(bi));
            String colorcode = "0x"+ rs + gs + bs;
            return colorcode;
        }

        private static String colorHex(Color c1, Color c2){
            final int LIMIT = 255;
            int ri = Math.min((c1.getRed() + c2.getRed()) / 2, LIMIT);
            int gi = Math.min((c1.getGreen() + c2.getGreen()) / 2, LIMIT);
            int bi = Math.min((c1.getBlue() + c2.getBlue()) / 2, LIMIT);
            String rs = String.format("%02x", Integer.valueOf(ri));
            String gs = String.format("%02x", Integer.valueOf(gi));
            String bs = String.format("%02x", Integer.valueOf(bi));
            String colorcode = "0x"+ rs + gs + bs;
            return colorcode;
        }

        public static Color convert(Color c1, Color c2){
            String new_color = colorHex(c1, c2);
            if(new_color.equals(colorHex(Color.yellow, Color.blue))) return Color.green;
            if(new_color.equals(colorHex(Color.red, Color.yellow))) return Color.orange;
            if(new_color.equals(colorHex(Color.red, Color.blue))) return Color.magenta;
            return Color.cyan;
        }
    }

    private double width, height, x, y;
    private Color color;

    public static Color convertColor(String s){
        Color c = Rectangle.INITIAL_COLOR; // Initial color is gray
        if(s.equals("red")) c = Color.red;
        else if(s.equals("blue")) c = Color.blue;
        else if(s.equals("yellow")) c = Color.yellow;
        return c;
    }
    
    public Rectangle(double width, double height, double x, double y){
        this.width = width;
        this.height = height;
        this.x = x;
        this.y = y;
        this.color = Rectangle.INITIAL_COLOR;
    }

    public Rectangle(Rectangle r){
        this(r.getWidth(), r.getHeight(), r.getX(), r.getY());
        this.color = r.getColor();
    }

    public Rectangle(double width, double height, double x, double y, String color){
        this(width, height, x, y);
        this.color = Rectangle.convertColor(color);
    }

    private Rectangle(double width, double height, double x, double y, Color color){
        this(width, height, x, y);
        this.color = color;
    }

    
    public double getWidth(){
        return this.width;
    }

    public double getHeight(){
        return this.height;
    }

    public double getX(){
        return this.x;
    }

    public double getY(){
        return this.y;
    }

    public double getLeftXAxis(){
        return this.x;
    }

    public double getRightXAxis(){
        return this.x + this.width;
    }

    public double getTopYAxis(){
        return this.y;
    }

    public double getBottomYAxis(){
        return this.y + this.height;
    }

    protected Color getColor(){
        return this.color;
    }

    // 移動した長方形を返す. この長方形自身は変形しない.
    public Rectangle getMovedRectangle(double mx, double my){
        return new Rectangle(this.width, this.height, this.x+(int)mx, this.y+(int)my, this.color);
    }    

    // 拡大した長方形を返す. この長方形自身は変形しない.
    public Rectangle getExpandRectangle(double mx, double my){
        return new Rectangle(Math.round(this.width*mx), Math.round(this.height*my), this.x, this.y, this.color);
    }

    /* 自身の長方形と与えられた長方形から重なった部分の長方形を新たに作成するメソッド.
    自身の長方形の中に与えられた長方形の一角でも入っていれば重なっていると判定し, 重なった長方形を作成する.
    作成したRectangleメソッドを返し, 作成できなかった場合はnullを返す. */
    public Rectangle getOverlapRetangle(Rectangle r){
        boolean lt = this.isPointInRectangle(r.getLeftXAxis(), r.getTopYAxis());
        boolean lb = this.isPointInRectangle(r.getLeftXAxis(), r.getBottomYAxis());
        boolean rt = this.isPointInRectangle(r.getRightXAxis(), r.getTopYAxis());
        boolean rb = this.isPointInRectangle(r.getRightXAxis(), r.getBottomYAxis());
        if(!(lt || lb || rt || rb)) return null; // 一角も入っていなければ作成不能.
        
        // 自身の長方形と与えられた長方形のx軸･y軸を昇順にsort.
        double[] x_lines = {
            this.getLeftXAxis(), this.getRightXAxis(), r.getLeftXAxis(), r.getRightXAxis()};
        double[] y_lines = {
            this.getTopYAxis(), this.getBottomYAxis(), r.getTopYAxis(), r.getBottomYAxis()};
        Arrays.sort(x_lines);
        Arrays.sort(y_lines);
        // 各4軸中の中2軸を新しい長方形の軸とする
        return new Rectangle(x_lines[2]-x_lines[1], y_lines[2]-y_lines[1], x_lines[1], y_lines[1], CalcColor.convert(this.getColor(), r.getColor()));
    }

    public boolean isPoint(){
        return (this.width == 0) && (this.height == 0);
    }   

    public boolean isLine(){
        boolean x_is_zero = (this.width == 0);
        boolean y_is_zero = (this.height == 0);
        return x_is_zero ^ y_is_zero;
    }

    public boolean isPointInRectangle(double x, double y){
        boolean x_axis_in = this.getLeftXAxis() < x && x < this.getRightXAxis();
        boolean y_axis_in = this.getTopYAxis() < y && y < this.getBottomYAxis();
        return x_axis_in && y_axis_in;
    }

    @Override
    public boolean equals(Object o){
        Rectangle r = (Rectangle)o;
        return ((this.width == r.getWidth()) && (this.height == r.getHeight()) && (this.x == r.getX()) && (this.y == r.getY())); 
    }

    @Override
    public int hashCode(){
        final int prime = 31;
        int result = 1;
        result = prime * result + (int)this.width;
        result = prime * result + (int)this.height;
        result = prime * result + (int)this.x;
        result = prime * result + (int)this.y;
        return result;
    }

    @Override
    public String toString(){
        return (
            "wide: " + this.width + "\n" + 
            "height: " + this.height + "\n" + 
            "Coordinate: (" + this.x + ", " + this.y + ")\n" +
            "color:" + CalcColor.colorHex(this.color) + "\n"
        );
    }
}
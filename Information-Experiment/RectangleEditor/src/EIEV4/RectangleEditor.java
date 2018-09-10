package EIEV4;

import EIEV4.Board;
import EIEV4.Command;
import EIEV4.Rectangle;
import EIEV4.DisplayCanvas;
import EIEV4.InputPanel;

import java.applet.Applet;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

import javax.swing.plaf.InputMapUIResource;


public class RectangleEditor extends Applet implements Runnable{
    private final int PANEL_HEIGHT = 50;

    private Board board;
    private Command command;

    private InputPanel input_panel;
    private DisplayCanvas display_canvas;

    private Thread thread;

    public RectangleEditor(){    
        this.board = new Board();
        this.command = new Command();
    }

    // 引数で与えられた座標にある長方形のIndexを返すメソッド. 存在しない場合は-1を返す.
    private int getIndex(Point p){
        return this.board.getRectangleIndex((int)p.x, (int)p.y);        
    }

    protected List<Rectangle> getRectangles(){
        return this.board.getRectangles();
    }

    protected boolean create(double width, double height, double x, double y){
        String color = this.input_panel.getColor();
        if(color == null) return false;
        return this.board.appendRectangle(new Rectangle(width, height, x, y, color));
    }

    protected boolean move(Point pressed, double dx, double dy){
        try{
            int index = this.getIndex(pressed);
            return this.board.moveRectangle(index, dx, dy);
        }catch(IndexOutOfBoundsException e){
            return false;
        }
    }

    protected boolean expand(Point pressed, Point released){
        try{
            int index = this.getIndex(pressed);
            Rectangle old_r = this.board.getRectangles().get(index);
            double mx = (released.x-old_r.getX()) / old_r.getWidth();
            double my = (released.y-old_r.getY()) / old_r.getHeight();
            return this.board.expandRectangle(index, mx, my);
        }catch(IndexOutOfBoundsException e){
            return false;
        }
    }

    protected boolean delete(Point pressed){
        try{
            int index = this.getIndex(pressed);
            return this.board.removeRectangle(index);
        }catch(IndexOutOfBoundsException e){
            return false;
        }
    }

    protected boolean deleteAll(){
        this.board.removeAllRectangles();
        return true;
    }

    protected boolean intersect(List<Point> points){
        List<Integer> indexes = new ArrayList<>();
        for(Point p: points){
            int i = this.getIndex(p);
            for(int j: indexes) if(j == i) continue;
            if(!this.board.existRectangle(i)) continue; 
            indexes.add(i);
        }
        if(indexes.size() < 2) return false;
        return this.board.intersect(indexes.get(0), indexes.get(1));
    }

    @Override
    public void init(){
        Dimension canvas_size = this.getSize();

        // 長方形を表示するcanvasのサイズをHTMLで定義されたwindowサイズにする
        this.board.setSize(canvas_size.width, canvas_size.height); 

        this.input_panel = new InputPanel(this);
        this.input_panel.setSize(this.getSize().width, PANEL_HEIGHT);
        this.display_canvas = new DisplayCanvas(this);
        this.display_canvas.setSize(canvas_size);

        this.setBackground(this.board.getColor());
        // ボタンが入ったboardを入れるために, windowを拡張
        this.resize(
            canvas_size.width,
            canvas_size.height + PANEL_HEIGHT
        );
        this.setLayout(new FlowLayout());

        this.add(this.input_panel);
        this.add(this.display_canvas);

        this.thread = new Thread(this);
        this.thread.start();
    }

    @Override
    public void start(){
        this.repaint();
    }

    @Override
    public void paint(Graphics g){
        this.input_panel.repaint();
        this.display_canvas.repaint();
    }

    @Override
    public void run(){
        while(this.command.isRunning()){
            this.command.exec(board);
            this.repaint();
        }
    }
    
    // cuiのみで実行する祭のmainメソッド
    public static void main(String args[]){
        RectangleEditor re = new RectangleEditor();
        re.run();
    }
}
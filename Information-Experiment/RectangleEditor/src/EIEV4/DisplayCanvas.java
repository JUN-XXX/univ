package EIEV4;

import EIEV4.Command;

import java.awt.Canvas;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.List;


class DisplayCanvas extends Canvas implements MouseListener{
    private RectangleEditor parent;
    private Point pressed_point;
    private Point released_point;
    private Point clicked_point;
    private List<Point> intersect_point_list;
    private Runnable exec_func_when_released;

    DisplayCanvas(RectangleEditor app){
        this.parent = app;
        
        // それぞれ, 最後にイベントが起きた際のPointを保存する変数.
        this.pressed_point = new Point();
        this.released_point = new Point();
        this.clicked_point = new Point();
        
        // intersect用の, 押されたPointを保存する配列.
        this.intersect_point_list = new ArrayList<>();
        
        this.addMouseListener(this);
    }

    private void callCreate(){
        this.parent.create(
            Math.abs(this.pressed_point.x - this.released_point.x),
            Math.abs(this.pressed_point.y - this.released_point.y),
            Math.min(this.pressed_point.x, this.released_point.x),
            Math.min(this.pressed_point.y, this.released_point.y)
        );
        this.repaint();
    }

    private void callMove(){
        int dx = this.released_point.x - this.pressed_point.x;
        int dy = this.released_point.y - this.pressed_point.y;
        this.parent.move(this.pressed_point, dx, dy);
        this.repaint();
    }

    private void callExpand(){
        this.parent.expand(this.pressed_point, this.released_point);
        this.repaint();
    }

    private void callDelete(){
        this.parent.delete(this.pressed_point);
        this.repaint();
    }

    private void callIntersect(){
        final int ALLOW_SIZE = 2;
        
        // intersect_point_listの要素数が1個になるまで要素を減らす
        while(this.intersect_point_list.size() >= ALLOW_SIZE)
            this.intersect_point_list.remove(0);

        // イベント発生時のPointを取得
        this.intersect_point_list.add(this.clicked_point);
        
        // intersectが成功したら, 保持しているPointをリセットする
        if(this.parent.intersect(this.intersect_point_list))
            this.intersect_point_list.clear();
        
        this.repaint();
    }

    @Override
    public void paint(Graphics g){
        g.drawRect(0, 0, this.getWidth()-1, this.getHeight()-1);
        
        for(Rectangle r: this.parent.getRectangles()){
            g.setColor(r.getColor());
            g.fillRect((int)r.getX(), (int)r.getY(), 
                       (int)r.getWidth(), (int)r.getHeight());
        }
    }

    @Override
    public void mouseEntered(MouseEvent e){
    }
  
    @Override
    public void mouseExited(MouseEvent e){
    }
  
    @Override
    public void mousePressed(MouseEvent e){
        this.pressed_point = e.getPoint();
        
        // 押されていたボタンに応じて, 離された際の実行メソッドを振り分ける
        if(e.isControlDown()) 
            this.exec_func_when_released = this::callMove;
        else if(e.isShiftDown()) 
            this.exec_func_when_released = this::callExpand;
        else if(e.isMetaDown()) 
            this.exec_func_when_released = this::callDelete;
        else 
            this.exec_func_when_released = this::callCreate;
        
    }
  
    @Override
    public void mouseReleased(MouseEvent e){
        this.released_point = e.getPoint();

        //事前に振り分けられていたメソッドを実行する.
        this.exec_func_when_released.run(); 
    }
  
    @Override
    public void mouseClicked(MouseEvent e){
        this.clicked_point = e.getPoint();

        if(e.isAltDown()){
            this.callIntersect();
        }
    }
}
package EIEV4;

import java.awt.Button;
import java.awt.Checkbox;
import java.awt.CheckboxGroup;
import java.awt.Panel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

class InputPanel extends Panel implements ActionListener{
    private RectangleEditor parent;
    private CheckboxGroup color_group;
    private List<Checkbox> checkboxes;
    private Button deleteall_button;

    InputPanel(RectangleEditor app){
        this.parent = app;
        this.color_group = new CheckboxGroup();
        this.checkboxes = new ArrayList<>();
        this.checkboxes.add(new Checkbox("gray", true, this.color_group));
        for(String s: new String[] {"red", "blue", "yellow"}) checkboxes.add(new Checkbox(s, false, this.color_group));
        this.deleteall_button = new Button("deleteAll");
        this.deleteall_button.addActionListener(this);

        for(Checkbox cb: checkboxes) this.add(cb);
        this.add(this.deleteall_button);
    }

    private void callDeleteAll(){
        this.parent.deleteAll();
        this.parent.repaint();
    }

    // ラジオボタンで選択されている色を取得する.
    public String getColor(){
        for(Checkbox cb: checkboxes) if(cb.getState()) 
            return cb.getLabel();
        
        return null;
    }

    @Override
    public void actionPerformed(ActionEvent e){
        if(e.getSource() == this.deleteall_button) this.callDeleteAll();
    }
}
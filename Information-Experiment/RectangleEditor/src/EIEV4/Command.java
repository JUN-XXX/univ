package EIEV4;

import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.lang.IndexOutOfBoundsException;
import java.util.List;

// RectangleEditorでの入力に対応するメソッド群

class Command{
    private BufferedReader br;
    private boolean is_running;

    public Command(){
        this.br =  new BufferedReader(new InputStreamReader(System.in));
        this.is_running = true;
    }

    // ボード上に長方形が1つも無かった場合に注意を促すメソッド
    private boolean hasRectangle(Board board){
        if(board.size() == 0){
            System.out.println("[Error] ボードに長方形がありません.");
            return false;
        }
        return true;
    }

    // ボードがこれ以上長方形を持つことができない場合に注意を促すメソッド
    private boolean canAppendRectangle(Board board){
        if(!board.canAppendRectangle()){
            System.out.println("[Error]これ以上ボードに長方形を追加できません.\n       不要な長方形を削除してください.");
            return false;
        }
        return true;
    }
        

    // 対象とする長方形をcuiで選択するメソッド.
    private int choiceRectangle(Board board) throws IndexOutOfBoundsException, IOException, NumberFormatException{
        int i = this.getInteger("対象とする長方形の番号を入力してください\n(displayBoardで見ることのできるrectangle numberを入力してください)");
        int index = i - 1;
        if(!board.existRectangle(index)) throw new IndexOutOfBoundsException();
        return index;
    }

    // 長方形をcuiで作成するメソッド
    private Rectangle createRectangle() throws IOException, NumberFormatException{
        double wide = this.getDouble("長方形の幅を入力してください.");
        if(wide <= 0){
            return null;
        }
        double height = this.getDouble("長方形の高さを入力してください.");
        if(height <= 0){
            return null;
        }
        double x = this.getDouble("長方形の左上のx座標を入力してください.");
        double y = this.getDouble("長方形の左上のy座標を入力してください.");
        String color = this.getString("長方形の色を入力してください\n{gray(defalut), red, blue, yellow}");
        
        Rectangle made_rectangle = new Rectangle(wide, height, x, y, color);
        return made_rectangle;
    }

    // 数値をcuiで入力するメソッド(int型)
    private int getInteger(String print_str) throws IOException, NumberFormatException{
        System.out.print(print_str + " : ");
        return Integer.parseInt(this.br.readLine());
    }

    // 数値をcuiで入力するメソッド(double型)
    private double getDouble(String print_str) throws IOException, NumberFormatException{
        System.out.print(print_str + " : ");
        return Double.parseDouble(this.br.readLine());
    }

    // 文字列をcuiで入力するメソッド
    private String getString(String print_str) throws IOException{
        System.out.print(print_str + " : ");
        return this.br.readLine();
    }

    public Board create(Board board) throws IOException, NumberFormatException{
        if(!this.canAppendRectangle(board)) return board;
        Rectangle made_rectangle = this.createRectangle();
        
        if(made_rectangle == null){
            System.out.println("[Error]入力された幅または高さをとることができません.");
            return board;
        } 
        if(board.appendRectangle(made_rectangle)){
            System.out.println("長方形の追加に成功しました.");
        }else{
            System.out.println("[Error]長方形の追加に失敗しました.長方形は追加されません.");
        }
        return board;
    }

    public Board move(Board board) throws IndexOutOfBoundsException, IOException, NumberFormatException{
        if(!this.hasRectangle(board)) return board;
        int index = this.choiceRectangle(board);
        double mx = this.getDouble("x軸方向へ移動させる量を入力してください.");
        double my = this.getDouble("y軸方向へ移動させる量をを入力してください.");
        
        if(board.moveRectangle(index, mx, my)) System.out.println("移動に成功しました.");
        else System.out.println("[Error]移動に失敗しました.");
        return board;
    }

    public Board expand(Board board) throws IndexOutOfBoundsException, IOException, NumberFormatException{
        if(!this.hasRectangle(board)) return board;
        int index = this.choiceRectangle(board);
        double mx = this.getDouble("x軸方向への拡大率を入力してください.");
        double my = this.getDouble("y軸方向への拡大率を入力してください.");
        if(board.expandRectangle(index, mx, my)) System.out.println("拡大に成功しました.");
        else System.out.println("拡大に失敗しました.");
        return board;
    }

    public Board shrink(Board board) throws IndexOutOfBoundsException, IOException, NumberFormatException{
        if(!this.hasRectangle(board)) return board;
        int index = this.choiceRectangle(board);
        double mx = this.getDouble("x軸方向への縮小率を入力してください.");
        double my = this.getDouble("y軸方向への縮小率を入力してください.");
        if(board.expandRectangle(index, 1/mx, 1/my)) System.out.println("縮小に成功しました.");
        else System.out.println("縮小に失敗しました.");
        return board;
    }

    public Board delete(Board board)throws IndexOutOfBoundsException, IOException, NumberFormatException{
        if(!this.hasRectangle(board)) return board;
        int index = this.choiceRectangle(board);
        if(board.removeRectangle(index)) System.out.println("長方形の削除に成功しました.");        
        else System.out.println("[Error]長方形の削除に失敗しました.");
        return board;
    }

    public Board deleteAll(Board board){
        if(!this.hasRectangle(board)) return board;
        if(board.removeAllRectangles()) System.out.println("長方形の削除に成功しました.");
        else System.out.println("[Error]長方形の削除に失敗しました.");              
        return board;
    }

    public Board intersect(Board board) throws IndexOutOfBoundsException, IOException{
        if(board.size() < 2){
            System.out.println("[Error]選択できる長方形が不足しています.");
            return board;
        }
        if(!this.canAppendRectangle(board)) return board;
            
        int index1 = this.choiceRectangle(board);
        int index2 = this.choiceRectangle(board);

        if(board.intersect(index1, index2)){
            System.out.println("長方形の追加に成功しました.");
        } else System.out.println("[Error]選択された2つの長方形からは作成されませんでした.");
        
        return board;
    }

    public boolean displayBoard(Board board){
        if(!this.hasRectangle(board)) return false;
        System.out.print(board.toString());
        return true;
    }

    public void exit(){
        System.out.println("プログラムを終了します.");
        this.is_running = false;
    }

    public void help(){
        String command_info =
            "\n" + "<コマンド一覧>" + "\n" +
            "create: 長方形を1つ作成します." + "\n" + 
            "move: 長方形を1つ移動します." + "\n" + 
            "expand: 長方形を1つ拡大します." + "\n" + 
            "shrink: 長方形を1つ縮小します." + "\n" + 
            "delete: 長方形を1つ削除します." + "\n" +
            "deleteAll: 長方形を全て削除します." + "\n" +
            "intersect: 2つの長方形の重なり部分を新しい長方形として作成します." + "\n" +
            "displayBoard: ボードに登録されている長方形を表示します." + "\n" +
            "help: コマンド一覧を表示します." + "\n" + 
            "exit: プログラムを終了します." + "\n";
        System.out.println(command_info);
    }

    // 終了コマンドが入力されていないことを確認するメソッド
    public boolean isRunning(){
        return this.is_running;
    }

    // コマンドを受け付けて処理を割り振るメソッド
    public void exec(Board board){
        try{
            this.help();

            System.out.print("コマンドを入力してください: ");
            String in_command = this.br.readLine();

            if (in_command.equals("exit")) this.exit();
            else if (in_command.equals("create")) this.create(board);
            else if (in_command.equals("move")) this.move(board);
            else if (in_command.equals("expand")) this.expand(board);
            else if (in_command.equals("shrink")) this.shrink(board);
            else if (in_command.equals("delete")) this.delete(board);
            else if (in_command.equals("deleteAll")) this.deleteAll(board);
            else if (in_command.equals("intersect")) this.intersect(board);
            else if (in_command.equals("displayBoard")) this.displayBoard(board);
            else if (in_command.equals("help")) return;
            else System.out.println("入力されたコマンドは存在しません.");
        }catch (IOException e){
            System.out.println("[Error]入力時にエラーが発生しました.");
        }catch(NumberFormatException e){
            System.out.println("[Error]数字以外の文字が入力されました.");
        }catch(IndexOutOfBoundsException e){
            System.out.println("[Error]配列外の数字が入力されました.");
        }
    }

    public void loop(Board board){
        while(true){
            this.exec(board);
        }
    }

}


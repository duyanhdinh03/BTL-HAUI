<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Form nhập hàng</title>
</head>
<body>
    <form action="nhaphang.php" method="post" enctype="multipart/form-data">
        <label>Mã hàng</label>
        <input type="text" name="code" id="code"/> <br>

        <label>Tên hàng</label>
        <input type="text" name="name" id="name"/> <br>
        
        <label>Số lượng</label>
        <input type="text" name="quantity" id="quantity"/> <br>

        <label>Đơn giá</label>
        <input type="text" name="price" id="price"/> <br>
        
        <label>Nhóm</label>
        <select name="category" id="category">
            <!-- chen option vao day -->
            <?php
                // mở file
                $fh = fopen("nhom.txt", "r") or die("Không đọc được file");

                // đọc từng dòng
                while(!feof($fh)){

                    //đọc 1 dòng
                    $line = fgets($fh);

                    if($line != ""){ // bỏ qua dòng trắng cuối cùng
                        // tách từng phần
                        $temp = explode("\t", $line);
                        
            ?>
                <option value="<?=$temp[0]?>"><?$temp[1]?></option>
            
                <?php 
                    }

                }
                fclose($fh);
            ?>
        </select>
        <br>
        <label>Ảnh hàng</label>
        <input type="file" name="image" id="image"/><br>

        <button type="submit">Lưu</button>
        <button type="reset">Xóa</button>
    </form>
</body>
</html>
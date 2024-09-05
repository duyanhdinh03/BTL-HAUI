<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Nhập thông tin của hàng</title>
</head>
<body>
    <form action="nhaphang.php" method="POST" enctype="multipart/form-data">
        <p>
            <label>Mã hàng</label>
            <input type="text" id="code" name="code"/>
        </p>
        <p>
            <label>Tên hàng</label>
            <input type="text" id="name" name="name"/>
        </p>
        <p>
            <label>Nhóm</label>
            <select name="category" id="category">
                <?php
                 $fh = fopen("category.txt", "r") or die("Không đọc được file");
                 while(!feof($fh)){
                     $line = fgets($fh);
                     if($line != ""){
                         $temp = explode("\t", $line);
                         
                     ?>
                     <option value="<?=$temp[0]?>"><?=$temp[1]?></option>
                     <?php
                     }
                 }
                 ?>
             </select>
        </p>
        <p>
            <label>Số lượng</label>
            <input  type="text" id="quantity" name="quantity" />
        </p>
        <p>
            <label>Đơn giá</label>
            <input type="text" id="price" name="price"/>
        </p>
            <label>Hình ảnh</label>
            <input  type="file" id="image" name="image" />
        </p>
        <p>
            <button type="submit">Nhập</button>
            <button type="reset">Cancel</button>
        </p>
    </form>
</body>
</html>
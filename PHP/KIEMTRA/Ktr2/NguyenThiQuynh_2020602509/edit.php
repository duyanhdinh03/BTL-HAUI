<?php 

$id = $_GET['id'];

include("connection.php");
$sql = "SELECT * FROM tblitems WHERE id = $id";
$result = mysqli_query($conn, $sql);
$row = mysqli_fetch_assoc($result);
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chỉnh sửa</title>
</head>
<body>
    <form action="saveEdit.php" method="post" enctype="multipart/form-data">
        <p>
            <label for="code">Mã hàng</label>
            <input type="text" id="code" name="code" value="<?=$row['code']?>"/>
        </p>
        <p>
            <label for="name">Tên hàng</label>
            <input type="text" id="name" name="name" value="<?=$row['name']?>"/>
        </p>
       
        <p>
            <label for="quantity">Số lượng</label>
            <input type="text" id="quantity" name="quantity" value="<?=$row['quantity']?>"/>
        </p>
        <p>
            <label for="price">Đơn giá</label>
            <input type="text" id="price" name="price" value="<?=$row['price']?>"/>
        </p>
        <p>
            <label for="image">Hình ảnh</label><br/>
            <img src="images/.<?=$row['imgpath']?>" id="output" width="150px"/><br/>
            <input type="file" id="image" name="image" onchange="loadFile(event)"/>
        </p>
        <p>
            <label for="grName">Nhóm</label>
            <select id="grName" name="grName">
                <?php 
                    include("connection.php");
                    $sqli = "SELECT * FROM tblcategory";
                    $rs = mysqli_query($conn, $sqli);
                    while($row = mysqli_fetch_assoc($rs)){
                    ?>
                        <option><?=$row['Name']?></option>
                    <?php
                    }
                    
                ?>
            </select>
        </p>
        <p>
            <input type="hidden" name="id" value="<?=$row['id']?>" />
            <button type="submit">Nhập</button>
            <button type="reset">Hủy</button>
        </p>
    </form>
    <? mysqli_close($conn); ?>

    <script>
        var loadFile = function(event){
            var image = document.getElementById('output');
            image.src = URL.createObjectURL(event.target.files[0]);
        }
    </script>
</body>
</html>
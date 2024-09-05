<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Input Products</title>
</head>
<body>
    <form action="inputProduct.php" method="post" enctype="multipart/form-data">
        <p>
            <label for="name">Tên hàng</label>
            <input type="text" id="name" name="name"/>
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
                        <option><?=$row['name']?></option>
                    <?php
                    }
                    
                    mysqli_close($conn);
                ?>
            </select>
        </p>
        <p>
            <label for="quantity">Số lượng</label>
            <input type="text" id="quantity" name="quantity"/>
        </p>
        <p>
            <label for="price">Đơn giá</label>
            <input type="text" id="price" name="price"/>
        </p>
        <p>
            <label for="image">Hình ảnh</label><br/>
            <img src="images/No.jpg" id="output" width="150px"/><br/>
            <input type="file" id="image" name="image" onchange="loadFile(event)"/>
        </p>
        <p>
            <button type="submit">Nhập</button>
            <button type="reset">Hủy</button>
        </p>
    </form>

    <script>
        var loadFile = function(event){
            var image = document.getElementById('output');
            image.src = URL.createObjectURL(event.target.files[0]);
        }
    </script>
</body>
</html>

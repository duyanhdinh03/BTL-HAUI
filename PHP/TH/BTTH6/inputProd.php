<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" 
    integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
    <title>Nhập hàng</title>
</head>
<body>
    <form action="handleProd.php" method="POST" class="sm-6">
        
        <label class="form-label">Tên hàng</label>
        <input type="text" id="name" name="name" ><br>

        <label class="form-label">Nhóm hàng</label>
        <select id="grName" name="grName">
        <?php
            include("configuration.php");
            $sql = "SELECT * FROM tblcategory";
            $rs = mysqli_query($conn, $sql);
            while($row = mysqli_fetch_assoc($rs)){
            ?>
            <option><?=$row['Name']?></option>
        <?php
            }
            ?>
        </select><br>

        <label class="form-label">Số lượng</label>
        <input type="int" id="quantity" name="quantity" ><br>

        <label class="form-label">Đơn giá</label>
        <input type="text" id="price" name="price"><br>

        <label class="form-label">Hình ảnh</label>        
        <img src="No.png" width="300px" id="output" /> <br/>
        <input type="file" id="image" name="image" onchange="loadFile(event)"><br>

        <button type="submit">Lưu</button>
    </form>

    <script>

        var loadFile = function (event){
            var imgae = document.getElementById('output');
            image.src = URL.createObjectURL(event.target.files[0]);
        }
    </script>
</body>
</html>
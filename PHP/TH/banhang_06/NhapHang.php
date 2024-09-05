<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
        * {
            font-size:30px;
            box-sizing: border-box;
            padding: 0;
            margin: 0;
        }
        table{
            width: 50%;
            margin:auto;
        }
        input[type="text"], textarea{
            width:100%;
            border:none;
            height:50px;
        }
        .button, button {
            font-size: 20px;
            width : 20%;
            height:50px;
            display: inline-block;
        }
    </style>
</head>
<body>
    <form action="./NhapHang2.php" method="post" enctype="multipart/form-data">
        <table border=1>
            <tr>
                <td colspan = "2" style="text-align:center"><h1>NHẬP MẶT HÀNG</h1></td>
            </tr>
            <tr>
                <td><label for="name">Tên hàng</label></td>
                <td><input type="text" name="name" id="name"></td>
            </tr>
            <tr>
            <td><label for="category">Loại hàng</label></td>
                <td>
                    <select name="category">
                    <?php
                        include "./connect.php";
                        $sql = "SELECT id,name FROM tblcategory";
                        $result = mysqli_query($conn, $sql);
                        if (mysqli_num_rows($result) > 0){
                            while($row = mysqli_fetch_assoc($result)){
                    ?>
                        <option value="<?=$row["id"]?>"><?=$row["name"]?></option>
                    <?php
                            }
                        }
                    ?>
                    </select>
                </td>
            </tr>
            <tr>
                <td><label for="quantity">Số lượng</label></td>
                <td><input type="text" name="quantity" id="quantity"></td>
            </tr>
            <tr>
                <td>
                    <label for="price">Đơn giá</label></td>
                    <td><input type="text" name="price" id="price">
                </td>
            </tr>
            <tr>
                <td><label for="image">Hình ảnh</label></td>
                <td><input type="file" name="image" id="image"></td>
            </tr>
            <tr>
                <td style = "text-align:center;" colspan="2">
                    <input type="submit" value="Nhập" class="button">
                    <input type="reset" value="Reset" class="button">
                    <a href="./GianHang.php"><button type="button">Mua hàng</button></a>
                    <a href="./NhapNhomHang.html"><button type="button">Quay lại</button></a>
                 </td>
            </tr>
        </table>
    </form>
</body>
</html>

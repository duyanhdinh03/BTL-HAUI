<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>List products</title>
    <style>
        table{
            border-collapse: collapse;
        }
        td{
            text-align: center;
        }
    </style>
</head>
<body>
    <button><a href="listcart.php">Giỏ hàng</a></button>
    <table border="1" width="100%">
        <tr>
            <th>Mã hàng</th>
            <th>Tên hàng</th>
            <th>Số lượng</th>
            <th>Đơn giá</th>
            <th>Hình ảnh</th>
            <th>Chọn mua</th>
        </tr>
        <?php
        include("connection.php");

        $sqli = "SELECT * FROM tblitems";
        $result = mysqli_query($conn, $sqli);
        while($row = mysqli_fetch_assoc($result)){
            ?>
            <tr>
                <td><?=$row['code']?></td>
                <td><?=$row['name']?></td>
                <td><?=$row['quantity']?></td>
                <td><?=$row['price']?></td>
                <td>
                    <img src="<?=$row['imgpath']?>" width="120px"/>
                </td>
                <td>
                    <button><a href="cart.php?id=<?=$row['id']?>&code=<?=$row['code']?>&name=<?=$row['name']?>&price=<?=$row['price']?>">Mua</a></button>
                    <button><a href="edit.php?id=<?=$row['id']?>">Sửa</a></button>
                    <button type="submit" onclick="Xacnhanxoa(<?=$row['id']?>)">Xóa</button>
                </td>
            </tr>
            <?php
        }        
        mysqli_close($conn);
        ?>
    </table>
    <form action="delete.php" method="post" id="formXoa">
        <input type="hidden" name="id" id="id"/>
    </form>
    <script type="text/javascript">
        function Xacnhanxoa(id){
            var str = "Bạn có thật sự muốn xóa không?";
            if(str){
                document.getElementById("id").value = id;
                document.getElementById("formXoa").submit();
            }
            alert(id);
        }
    </script>
</body>
</html>
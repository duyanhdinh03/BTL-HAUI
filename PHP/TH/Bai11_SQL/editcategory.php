<?php
$id = $_GET['id'];

include("configuration.php");
$conn = mysqli_connect($url, $username, $password, $dbname);
if(!$conn){ // ket noi that bai
    die("Kết nối không thành công ".mysqli_connect_error());
}
$sql = "SELECT * FROM tblcategory WHERE id = $id";
$result = mysqli_query($conn, $sql);
$row = mysqli_fetch_assoc($result);
mysqli_close($conn);
header("Location:listcategory.php");
?>
<form action="saveEdit.php" method="post">
    <p>
        <label>Tên nhóm: </label> 
        <input type="text" required="name" name="name" id="name" value="<?=$row['name']?>"/>
    </p>
    <p>
        <label>Mô tả</label>
        <textarea name="desc" id="desc" cols="50" rows="10"  value="<?=$row['desc']?>"></textarea>>
    </p>
    <p>
        <input type="hidden" name="id" value="<?=$row['id']?>" />
        <button type="submit">Lưu</button>
        <button type="reset">Xóa</button>
    </p>
</form>

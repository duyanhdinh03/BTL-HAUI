<?php 
    $id  = $_POST['id'];
    include("connection.php");

    $sql = "DELETE FROM tblitems WHERE id = $id";
    
    mysqli_query($conn, $sql);
    mysqli_close($conn);
    header("Location: listProduct.php");

?>
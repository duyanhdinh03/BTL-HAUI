<?php 


$name = $_POST["name"];
$desc = $_POST["description"];

include("connection.php");

$sqli = "INSERT INTO tblcategory(name, description) VALUES ('$name', '$desc')";
mysqli_query($conn, $sqli);

mysqli_close($conn);
header("Location:inputCate.html");

?>
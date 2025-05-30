<?php
session_start();
$conn = mysqli_connect("localhost", "root", "", "cykor_db");

if (!$conn) {
    die("DB fail: " . mysqli_connect_error());
}

if (!isset($_GET['title'])) {
    die("Nothing to view.");
}

$title = $_GET['title'];

$sql = "SELECT * FROM posts WHERE title = '$title'";
$result = mysqli_query($conn, $sql);

$post = mysqli_fetch_assoc($result);
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Cykor Week 3</title>
</head>
<body>
    <h1><?php echo htmlspecialchars($post['title']); ?></h1>
    <p><strong>Writer:</strong> <?php echo htmlspecialchars($post['writer']); ?></p>
    <hr>
    <p><?php echo nl2br(htmlspecialchars($post['content'])); ?></p>
    <hr>

    <?php
    if (isset($_SESSION['username']) && $_SESSION['username'] === $post['writer']) {
        echo "<a href='edit.php?title=" . $post['title'] . "'><button>Edit</button></a>";
        echo "<a href='delete.php?title=" . $post['title'] . "'><button>삭제하기</button></a>";
    }
    ?>
    <br><br>
    <a href="list.php">Back to list</a>
</body>
</html>

<?php
mysqli_close($conn);
?>

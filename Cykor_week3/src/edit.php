<?php
session_start();
$conn = mysqli_connect("localhost", "root", "1234", "cykor_db");

if (!$conn) {
    die("DB fail: " . mysqli_connect_error());
}

if (!isset($_GET['title'])) {
    die("Noting to edit.");
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
    <h2>Edit</h2>
    <form action="edit_process.php" method="POST">
        <input type="hidden" name="title" value="<?php echo $post['title']; ?>">
        <label>Title:</label><br>
        <input type="text" name="new_title" value="<?php echo $post['title']; ?>"><br><br>
        <label>Content:</label><br>
        <textarea name="content" rows="10" cols="50"><?php echo $post['content']; ?></textarea><br><br>
        <button type="submit">Edit</button>
    </form>
    <br>
    <a href="view.php?title=<?php echo urlencode($post['title']); ?>">Back to list</a>
</body>
</html>

<?php
mysqli_close($conn);
?>

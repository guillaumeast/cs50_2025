SELECT DISTINCT p.name
FROM people AS p
JOIN stars  AS s ON s.person_id = p.id
JOIN movies AS m ON m.id = s.movie_id
WHERE m.id IN (
	SELECT m.id
	FROM movies AS m
	JOIN stars  AS s ON s.movie_id = m.id
	JOIN people AS p ON p.id = s.person_id
	WHERE p.name = 'Kevin Bacon' AND p.birth = 1958
)
AND p.id != (
	SELECT id
	FROM people
	WHERE name = 'Kevin Bacon' AND birth = 1958
)
;

SELECT m.title, r.rating
FROM movies  AS m
JOIN ratings AS r ON r.movie_id = m.id
WHERE m.year = 2010
ORDER BY r.rating DESC, m.title ASC
;

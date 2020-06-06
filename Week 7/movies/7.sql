SELECT m.title, r.rating
FROM movies m
INNER JOIN ratings r
    ON m.id = r.movie_id
WHERE m.year = 2010 AND r.rating >= 0.0
ORDER BY r.rating DESC, m.title ASC;

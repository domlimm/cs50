SELECT AVG(rating) as 'Avg Rating (2012)'
FROM ratings r
INNER JOIN movies m
    ON r.movie_id = m.id
WHERE m.year = 2012;

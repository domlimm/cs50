SELECT m.title
FROM movies m
INNER JOIN stars s
    ON m.id = s.movie_id
INNER JOIN people p
    ON s.person_id = p.id
WHERE p.name = 'Johnny Depp'

INTERSECT

SELECT m.title
FROM movies m
INNER JOIN stars s
    ON m.id = s.movie_id
INNER JOIN people p
    ON s.person_id = p.id
WHERE p.name = 'Helena Bonham Carter'

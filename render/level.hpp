#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "../assets/default_textures.hpp"

#define TILE_SCALE 3.f

#define TRUE_TILE_SIZE(axis) (TILE_SCALE * this->tileSize.##axis )

extern const int oldLevel[] =
{
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
    0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
    0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
    2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
    0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
};
extern const int level[] =
{
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3,
    1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 3,
    4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 4, 4, 4, 3,
    4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 3,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0,
};

template<class T> sf::Vector2<T> operator/(sf::Vector2<T> l, sf::Vector2<T> r) {
    return sf::Vector2<T>(l.x / r.x, l.y / r.y);
}

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    sf::Vector2u tileSize;
    const int* tiles;
    unsigned int width;
    unsigned int height;
public:

    bool load(const char* tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        this->setPosition(sf::Vector2f(0, 0));
        this->tiles = tiles;
        this->tileSize = tileSize;
        this->width = width;
        this->height = height;
        // load the tileset texture
        if (!m_tileset.LOAD(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i) {
            for (unsigned int j = 0; j < height; ++j) {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        }
        setScale(TILE_SCALE, TILE_SCALE);
        return true;
    }

    std::array<std::array<int, 2>, 4> GetCloseColliders(const sf::RectangleShape& collider) const {
        std::array<std::array<int, 2>, 4> collided = {{ {{ -1, -1 }}, {{ -1, -1 }}, {{ -1, -1 }}, {{ -1, -1 }} }};
        const sf::FloatRect trueMapSize(0.f, 0.f, this->width * TRUE_TILE_SIZE(x), this->height * TRUE_TILE_SIZE(y));
        const sf::Vector2f& colliderPosition = collider.getPosition();
        collided[0] = trueMapSize.contains(collider.getPosition()                                           ) ? std::array<int, 2>{{ this->tiles[((int)((colliderPosition.x                         ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y                       ) / TRUE_TILE_SIZE(y))) * (int)this->width], ((int)((colliderPosition.x                         ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y                       ) / TRUE_TILE_SIZE(y))) * (int)this->width }} : std::array<int, 2>{{-1, -1}};
        collided[1] = trueMapSize.contains(collider.getPosition() + sf::Vector2f(collider.getSize().x, 0.f) ) ? std::array<int, 2>{{ this->tiles[((int)((colliderPosition.x + collider.getSize().x  ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y                       ) / TRUE_TILE_SIZE(y))) * (int)this->width], ((int)((colliderPosition.x + collider.getSize().x  ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y                       ) / TRUE_TILE_SIZE(y))) * (int)this->width }} : std::array<int, 2>{{-1, -1}};
        collided[2] = trueMapSize.contains(collider.getPosition() + sf::Vector2f(0.f, collider.getSize().y) ) ? std::array<int, 2>{{ this->tiles[((int)((colliderPosition.x                         ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y + collider.getSize().y) / TRUE_TILE_SIZE(y))) * (int)this->width], ((int)((colliderPosition.x                         ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y + collider.getSize().y) / TRUE_TILE_SIZE(y))) * (int)this->width }} : std::array<int, 2>{{-1, -1}};
        collided[3] = trueMapSize.contains(collider.getPosition() + collider.getSize()                      ) ? std::array<int, 2>{{ this->tiles[((int)((colliderPosition.x + collider.getSize().x  ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y + collider.getSize().y) / TRUE_TILE_SIZE(y))) * (int)this->width], ((int)((colliderPosition.x + collider.getSize().x  ) / TRUE_TILE_SIZE(x))) + /*y*/((int)((colliderPosition.y + collider.getSize().y) / TRUE_TILE_SIZE(y))) * (int)this->width }} : std::array<int, 2>{{-1, -1}};
        return collided;
    }

    int GetIndex(const sf::Vector2f& pos) { //find index in 1d array which position is in [NOT SAFE FROM OUT OF BOUNDS]
        //return (pos.y / (TILE_SCALE * this->tileSize.y)) * this->width + (pos.x / (TILE_SCALE * this->tileSize.x));
        //ypos * x + xpos = index
        return (int)(pos.y / TRUE_TILE_SIZE(y)) * this->width + (int)(pos.x / TRUE_TILE_SIZE(x));
    }
    sf::Vector2f GetPositionFromIndex(int index) { //turn 1d index in array into 2d position of a tile [NOT SAFE FROM OUT OF BOUNDS]
        //return sf::Vector2f((index % this->width) * TRUE_TILE_SIZE(x), (index % this->height) * TRUE_TILE_SIZE(y));
        return sf::Vector2f((index % this->width) * TRUE_TILE_SIZE(x),
            ((int)(index / this->width)) * TRUE_TILE_SIZE(y));
    }

    static std::array<std::vector<sf::Vector2f>, 4> GenetateColliderPoints(std::array<std::array<int, 2>, 4> colliders, TileMap& map) {
        /*
        //this needs to be able to create custom colliders that will be programmed in here:
        //no collider
        //full tile
        //half tile (top)
        //half tile (bottom)
        //half tile (left)
        //half tile (right)
        //slopes facing left up
        //slopes facing left down
        //slopes facing right up
        //slopes facing right down
        //spike facing top
        //spike facing bottom
        //spike facing left
        //spike facing right
        //
        // crossed off when completed
        */
        //////////////////////////////////

        /*
        ABOVE CAN BE BROKEN DOWN INTO BASIC COMPONENTS TO BE ROTATED:
        no collider
        full tile
        half tile
        slope
        spikes
        */

        std::array<std::vector<sf::Vector2f>, 4> colliderShapes;

        for (char i = (char)0; i < (char)4; i++) {
            switch (colliders[i][0]) {
            case 0: case 3: { //full tile
                colliderShapes[i].push_back(sf::Vector2f((colliders[i][1] % map.width) * map.tileSize.x * TILE_SCALE                    , (colliders[i][1] / map.width) * map.tileSize.y * TILE_SCALE                  )); //TL
                colliderShapes[i].push_back(sf::Vector2f((colliders[i][1] % map.width) * map.tileSize.x * TILE_SCALE + map.tileSize.x   , (colliders[i][1] / map.width) * map.tileSize.y * TILE_SCALE                  )); //TR
                colliderShapes[i].push_back(sf::Vector2f((colliders[i][1] % map.width) * map.tileSize.x * TILE_SCALE                    , (colliders[i][1] / map.width) * map.tileSize.y * TILE_SCALE + map.tileSize.y )); //BL
                colliderShapes[i].push_back(sf::Vector2f((colliders[i][1] % map.width) * map.tileSize.x * TILE_SCALE + map.tileSize.x   , (colliders[i][1] / map.width) * map.tileSize.y * TILE_SCALE + map.tileSize.y )); //BR
                break;
            }
            case -3: { //half tile
                break;
            }
            case -4: { //slope tile
                break;
            }
            case 2: { //spike tile
                colliderShapes[i] = { { sf::Vector2f(-1.f, -1.f) } };
                break;
            }
            default: //no collider
                colliderShapes[i] = { { sf::Vector2f(-1.f, -1.f) } };
                break;
            }
        }
        return colliderShapes;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

namespace cldr {
    class ColliderShape {
    private:
        std::vector<sf::Vector2f> data;
        int vertexCount;
        bool (*CorrectVertexFunction)(std::vector<sf::Vector2f>&, sf::FloatRect&);
        bool (*CorrectVertexFunctionPoints)(std::vector<sf::Vector2f>&, sf::Vector2f&);
    public:
        ColliderShape() = default;
        ColliderShape(std::vector<sf::Vector2f> points) : data(points) {
            vertexCount = points.size();
            this->CorrectVertexFunction = vertexCount == 4 ? &ColliderShape::CorrectVertexFunctionForQuads : &ColliderShape::CorrectVertexFunctionForTris;
            this->CorrectVertexFunctionPoints = vertexCount == 4 ? &ColliderShape::CorrectVertexFunctionPointsForQuads : &ColliderShape::CorrectVertexFunctionPointsForTris;
        }
        bool Collided(sf::FloatRect collider) { //this will be the players bounding box
            return this->CorrectVertexFunction(this->data, collider);
        }
        bool Collided(sf::Vector2f collider) { //this will be the players bounding box
            return this->CorrectVertexFunctionPoints(this->data, collider);
        }


        static bool CorrectVertexFunctionForQuads(std::vector<sf::Vector2f>& shape, sf::FloatRect& collider) {
            //simple FloatRect collision
            return sf::FloatRect(shape[0], sf::Vector2f(shape[3].x - shape[0].x, shape[3].y - shape[0].y)).intersects(collider);
        }
        static bool CorrectVertexFunctionForTris(std::vector<sf::Vector2f>&, sf::FloatRect&) {
            return false; //write at some point you turd
        }


        static bool CorrectVertexFunctionPointsForQuads(std::vector<sf::Vector2f>& shape, sf::Vector2f& collider) {
            //simple FloatRect collision.... and you still cant do it you dumb fuck
            return sf::FloatRect(shape[0], sf::Vector2f(shape[3].x - shape[0].x, shape[3].y - shape[0].y)).contains(collider);
        }
        static bool CorrectVertexFunctionPointsForTris(std::vector<sf::Vector2f>& shape, sf::Vector2f& collider) {
            return false; //WHY ISNT IT WRITTEN YOU LAZY TWAT
        }
    };
}

#endif
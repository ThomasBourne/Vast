#ifndef PARTICALS_HPP
#define PARTICALS_HPP

#include <SFML/Graphics.hpp>
namespace particle {
    /*struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Time lifetime;

        void update(sf::Time elapsed)
        {
            //update the particle lifetime
            this->lifetime -= elapsed;

            // if the particle is dead, respawn it
            if (this->lifetime <= sf::Time::Zero)
                resetParticle();

            // update the position of the corresponding vertex
            this->position += this->velocity * elapsed.asSeconds();
        }
        void resetParticle(sf::Vector2f& pos)
        {
            // give a random velocity and lifetime to the particle
            float angle = (std::rand() % 360) * 3.14f / 180.f;
            float speed = (std::rand() % 50) + 50.f;
            this->velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
            this->lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

            // reset the position of the corresponding vertex
            this->position = pos;
        }
    };*/
    class ParticleSystem : public sf::Drawable, public sf::Transformable //class copied directly from sfml website
    {
    public:

        ParticleSystem(unsigned int count, float duration) :
            m_particles(count),
            m_vertices(sf::Points, count),
            m_lifetime(sf::seconds(duration)),
            m_emitter(0.f, 0.f)
        {
        }

        void setEmitter(sf::Vector2f position)
        {
            m_emitter = position;
        }

        void update(sf::Time elapsed)
        {
            for (std::size_t i = 0; i < m_particles.size(); ++i)
            {
                // update the particle lifetime
                Particle& p = m_particles[i];
                p.lifetime -= elapsed;

                // if the particle is dead, respawn it
                if (p.lifetime <= sf::Time::Zero)
                    resetParticle(i);

                // update the position of the corresponding vertex
                m_vertices[i].position += p.velocity * elapsed.asSeconds();

                // update the alpha (transparency) of the particle according to its lifetime
                float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
                m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
            }
        }

    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            // apply the transform
            states.transform *= getTransform();

            // our particles don't use a texture
            states.texture = NULL;

            // draw the vertex array
            target.draw(m_vertices, states);
        }

    public:
        struct Particle
        {
            sf::Vector2f velocity;
            sf::Time lifetime;
        };

    private:

        void resetParticle(std::size_t index)
        {
            // give a random velocity and lifetime to the particle
            float angle = (std::rand() % 360) * 3.14f / 180.f;
            float speed = (std::rand() % 50) + 50.f;
            m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
            m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

            // reset the position of the corresponding vertex
            m_vertices[index].position = m_emitter;
        }

        std::vector<Particle> m_particles;
        sf::VertexArray m_vertices;
        sf::Time m_lifetime;
        sf::Vector2f m_emitter;
    };

    /*class ParticlePool {
        int poolSize = 0;
        ParticleSystem::Particle* firstAvailable;
        ParticleSystem::Particle* particleArray = nullptr;
    public:
        ParticlePool(ParticleSystem* emitter);
        virtual ~ParticlePool();

        //Generates a new particle each time it's called
        void Generate(sf::Vector2f pos, sf::Vector2f vel, pRect);

        // Update (move and draw) particles in the pool. If there are no particles alive returns false
        bool Update(float dt);
    }*/
}
#endif
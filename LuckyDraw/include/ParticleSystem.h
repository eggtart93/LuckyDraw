#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>


class ParticleSystem : public sf::Drawable, public sf::Transformable
{
    public:
    enum ParticleType
    {
        Coin,
        Star,
        Circle,
        Banana,
        Gold
    };
    enum EmitterType
    {
        Drop,
        FloatUp,
        FloatDown
    };

    public:
    ParticleSystem(int pType, unsigned int qt, const TextureHolder& textures);

    void update(sf::Time dt);
    void emit(bool flag);
    void setEmitter(sf::Vector2f position, int emitterType);
    void selectParticle(int particleType);
    void setLifeTime(float duration);
    void setEmitSpeed(float speed);
    void setParticleQt(unsigned int qt);
    int getParticleType() const;
    bool isAllDismissed() const;

    private:
    struct Particle
    {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Color color;
        sf::Time lifetime;
    };

    private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
    void computeVertices() const;

    bool drop(sf::Time dt);
    bool floatUp(sf::Time dt);

    void applyGravity(Particle& p);

    const sf::Texture* getTexture() const;

    private:
    std::vector<Particle> m_particles;
    mutable sf::VertexArray m_vertices;
    const TextureHolder& m_textures;
    int m_particle_type;
    int m_emitter_type;

    float m_speed;
    sf::Time m_lifetime;
    sf::Vector2f m_emitter;
    sf::Color* m_p_color;
    std::vector<sf::Color> mColorSet;
    unsigned int m_color_index;
    float m_time_buf;
    bool m_needed_update;
    bool m_all_dismissed;
};

#endif // PARTICLESYSTEM_H

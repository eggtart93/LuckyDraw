#include "ParticleSystem.h"

#define GRAVITY_ACCELERATE_SPEED 10.f
#define VAR_SPEED 55
#define PARTICLES_QT 380
#define VAR_LIFE_TIME 3500  // in ms
#define MIN_LIFE_TIME 1000 // in ms
#define COLOR_CHANGE_TIME 1.2f // in sec
ParticleSystem::ParticleSystem(int pType, unsigned int qt, const TextureHolder& textures)
:   m_particles(qt),
    m_vertices(sf::Quads),
    m_textures(textures),
    m_particle_type(pType),
    m_lifetime(sf::milliseconds(VAR_LIFE_TIME)),
    m_emitter(0, 0),
    m_p_color(nullptr),
    mColorSet(),
    m_time_buf(0.f),
    m_needed_update(false)
{
    m_color_index = 0;
    mColorSet.push_back(sf::Color(0, 0, 255, 255));
    mColorSet.push_back(sf::Color(30, 45, 255, 255));
    mColorSet.push_back(sf::Color(34, 167, 240, 255));
    
    mColorSet.push_back(sf::Color(150, 45, 255, 255));
    mColorSet.push_back(sf::Color(190, 45, 255, 255));
    mColorSet.push_back(sf::Color(230, 45, 255, 255));

    mColorSet.push_back(sf::Color(255, 50, 150, 255));
    mColorSet.push_back(sf::Color(255, 45, 90, 255));
    mColorSet.push_back(sf::Color(255, 45, 50, 255));

    mColorSet.push_back(sf::Color(255, 140, 59, 255));
    mColorSet.push_back(sf::Color(248, 190, 6, 255));
    mColorSet.push_back(sf::Color(255, 240, 30, 255));

    mColorSet.push_back(sf::Color(180, 255, 40, 255));
    mColorSet.push_back(sf::Color(80, 240, 30, 255));
    mColorSet.push_back(sf::Color(0, 230, 10, 255));

    m_p_color = &mColorSet[m_color_index];
}

void ParticleSystem::setEmitter(sf::Vector2f pos, int emitterType)
{
    m_emitter = pos;
    m_emitter_type = emitterType;
}

void ParticleSystem::update(sf::Time dt)
{
    if (!m_needed_update && m_all_dismissed) { return; }

    switch (m_emitter_type)
    {
    case Drop:
        m_all_dismissed = drop(dt);
        break;

    case FloatUp:
        m_all_dismissed = floatUp(dt);
        break;

    case FloatDown:
        break;

    default:
        break;
    }


}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_all_dismissed || m_needed_update)
    {
        // apply the transform
        states.transform *= getTransform();

        computeVertices();

        states.texture = getTexture();

        // draw the vertex array
        target.draw(m_vertices, states);
    }
}

void ParticleSystem::addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
    if (m_particle_type == Circle || m_particle_type == Star) vertex.color = color;

	m_vertices.append(vertex);
}

void ParticleSystem::computeVertices() const
{
	sf::Vector2f size(getTexture()->getSize());
	sf::Vector2f half = size / 2.f;

	// Refill vertex array
	m_vertices.clear();
	for(const Particle& particle : m_particles)
	{
		sf::Vector2f pos = particle.position;

		float ratio = particle.lifetime.asSeconds() / m_lifetime.asSeconds();

        sf::Color color = particle.color;
        color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

        addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
        addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
        addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
        addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
	}
}

void ParticleSystem::applyGravity(Particle& p)
{
    p.velocity.y += GRAVITY_ACCELERATE_SPEED;
}

bool ParticleSystem::drop(sf::Time dt)
{
    bool allDismissed = true;

    if (m_needed_update)
    {
        m_time_buf += dt.asSeconds();
        if(m_time_buf > 1.5f)
        {
            m_needed_update = false;
            m_time_buf = 0.f;
        }
    }

    for (Particle& p : m_particles)
    {
        p.lifetime -= dt;
        allDismissed = allDismissed && p.lifetime <= sf::Time::Zero;
        if (m_needed_update && p.lifetime <= sf::Time::Zero)
        {
            float angle = (20 + std::rand() % 140) * 3.14f / 180.f;
            float speed = (std::rand() % VAR_SPEED) + m_speed;
            p.velocity = sf::Vector2f(std::cos(angle) * speed, -std::abs(std::sin(angle)) * speed);
            p.lifetime = sf::milliseconds((std::rand() % m_lifetime.asMilliseconds()) + MIN_LIFE_TIME);
            
            if (m_particle_type != Coin && m_particle_type != Banana)
            {
                /* Yellow
                p.color = sf::Color(std::rand() % 45 + 210,
                std::rand() % 135 + 120,
                30,
                255);
                */
                
                p.color = sf::Color(std::rand() % 245 + 10,
                std::rand() % 245 + 10,
                std::rand() % 245 + 10,
                255);
            }
            p.position = m_emitter;
        }

        applyGravity(p);

        p.position += p.velocity * dt.asSeconds();
    }
    return allDismissed;
}

bool ParticleSystem::floatUp(sf::Time dt)
{
    sf::Vector2f emit_pos = m_emitter;
    emit_pos.x = std::rand() % (unsigned int)(m_emitter.x);

    if (m_time_buf > COLOR_CHANGE_TIME)
    {
        m_time_buf = 0.f;
        m_color_index++;
        if (m_color_index >= mColorSet.size()) { m_color_index = 0; }
        m_p_color = &mColorSet[m_color_index];
    }
    m_time_buf += dt.asSeconds();

    bool allDismissed = true;
    for (Particle& p : m_particles)
    {
        p.lifetime -= dt;
        allDismissed = allDismissed && p.lifetime <= sf::Time::Zero;
        if (m_needed_update && p.lifetime <= sf::Time::Zero)
        {
            float angle = (std::rand() % 360) * 3.14f / 180.f;
            float speed = (std::rand() % VAR_SPEED) + m_speed;
            p.velocity = sf::Vector2f(std::cos(angle) * speed, -std::abs(std::sin(angle)) * speed);
            p.lifetime = sf::milliseconds((std::rand() % m_lifetime.asMilliseconds()) + MIN_LIFE_TIME);
            p.color = *m_p_color;
//            p.color = sf::Color(std::rand() % 255 + 45,
//                                std::rand() % 255 + 40,
//                                std::rand() % 255 + 30,
//                                255);
            p.position = emit_pos;
        }

        p.position += p.velocity * dt.asSeconds();
    }
    return allDismissed;
}

void ParticleSystem::setLifeTime(float duration)
{
    m_lifetime = sf::seconds(duration);
}

void ParticleSystem::setEmitSpeed(float speed)
{
    m_speed = speed;
}

void ParticleSystem::emit(bool flag)
{
    m_needed_update = flag;
}

void ParticleSystem::selectParticle(int particleType)
{
    m_particle_type = particleType;
}

void ParticleSystem::setParticleQt(unsigned int qt)
{
    m_particles.resize(qt);
}

bool ParticleSystem::isAllDismissed() const
{
    return m_all_dismissed;
}

int ParticleSystem::getParticleType() const
{
    return m_particle_type;
}

const sf::Texture* ParticleSystem::getTexture() const
{
    switch (m_particle_type)
    {
    case Coin:
        return &m_textures.get(Textures::Coin);
        break;

    case Star:
        return &m_textures.get(Textures::Star);
        break;

    case Banana:
        return &m_textures.get(Textures::Banana);
        break;
        
    case Gold:
        return &m_textures.get(Textures::Gold);
        break;

    case Circle:
    default:
        return &m_textures.get(Textures::Particle);
        break;
    }
}

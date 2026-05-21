#include <Geode/Geode.hpp>
#include <Geode/modify/ParticleGameObject.hpp>

#include <algorithm>
#include <cmath>

using namespace geode::prelude;


float getMultiplier(char const* key) {
    return std::max(Mod::get()->getSettingValue<float>(key), 0.f);
}

float scalePositiveValue(float value, float scale) {
    return value < 0.f ? value : value * scale;
}

unsigned int scaleParticleCount(unsigned int count, float scale) {
    auto maxParticles = std::max<int64_t>(
        Mod::get()->getSettingValue<int64_t>("max-particles-per-emitter"),
        1
    );
    auto scaledCount = std::llround(count * scale);

    return static_cast<unsigned int>(std::clamp<int64_t>(scaledCount, 1, maxParticles));
}


class $modify(ParticlesMulty, ParticleGameObject) {
    void applyParticleSettings(cocos2d::CCParticleSystemQuad* particle) {
        if (!particle) {
            return;
        }

        ParticleGameObject::applyParticleSettings(particle);

        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            return;
        }

        auto countMultiplier = getMultiplier("particle-count-multiplier");
        particle->setTotalParticles(scaleParticleCount(particle->getTotalParticles(), countMultiplier));
        particle->setEmissionRate(std::max(particle->getEmissionRate() * countMultiplier, 0.f));

        auto sizeMultiplier = getMultiplier("size-multiplier");
        particle->setStartSize(scalePositiveValue(particle->getStartSize(), sizeMultiplier));
        particle->setStartSizeVar(scalePositiveValue(particle->getStartSizeVar(), sizeMultiplier));
        particle->setEndSize(scalePositiveValue(particle->getEndSize(), sizeMultiplier));
        particle->setEndSizeVar(scalePositiveValue(particle->getEndSizeVar(), sizeMultiplier));

        auto lifeMultiplier = getMultiplier("life-multiplier");
        particle->setLife(std::max(particle->getLife() * lifeMultiplier, 0.f));
        particle->setLifeVar(std::max(particle->getLifeVar() * lifeMultiplier, 0.f));

        auto speedMultiplier = getMultiplier("speed-multiplier");
        particle->setSpeed(particle->getSpeed() * speedMultiplier);
        particle->setSpeedVar(particle->getSpeedVar() * speedMultiplier);
        particle->setRadialAccel(particle->getRadialAccel() * speedMultiplier);
        particle->setRadialAccelVar(particle->getRadialAccelVar() * speedMultiplier);
        particle->setTangentialAccel(particle->getTangentialAccel() * speedMultiplier);
        particle->setTangentialAccelVar(particle->getTangentialAccelVar() * speedMultiplier);
        particle->setStartRadius(scalePositiveValue(particle->getStartRadius(), speedMultiplier));
        particle->setStartRadiusVar(scalePositiveValue(particle->getStartRadiusVar(), speedMultiplier));
        particle->setEndRadius(scalePositiveValue(particle->getEndRadius(), speedMultiplier));
        particle->setEndRadiusVar(scalePositiveValue(particle->getEndRadiusVar(), speedMultiplier));

        auto gravityMultiplier = getMultiplier("gravity-multiplier");
        auto gravity = particle->getGravity();
        particle->setGravity(ccp(gravity.x * gravityMultiplier, gravity.y * gravityMultiplier));

        auto rotationMultiplier = getMultiplier("rotation-multiplier");
        particle->setStartSpin(particle->getStartSpin() * rotationMultiplier);
        particle->setStartSpinVar(particle->getStartSpinVar() * rotationMultiplier);
        particle->setEndSpin(particle->getEndSpin() * rotationMultiplier);
        particle->setEndSpinVar(particle->getEndSpinVar() * rotationMultiplier);
        particle->setRotatePerSecond(particle->getRotatePerSecond() * rotationMultiplier);
        particle->setRotatePerSecondVar(particle->getRotatePerSecondVar() * rotationMultiplier);

        particle->saveDefaults();
    }
};

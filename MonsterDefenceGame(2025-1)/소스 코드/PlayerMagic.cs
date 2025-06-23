using System.Threading;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerMagic : MonoBehaviour
{
    public GameObject magic1Prefab;
    public GameObject magic2Prefab;
    public GameObject magic3Prefab;
    public GameObject magic3EffectPrefab;
    public Transform firePoint;
    public SkillUIController skillUI;

    private float magic1Cooldown = 0.2f;
    private float magic2Cooldown = 1.0f;
    private float magic3Cooldown = 3.0f;

    private float lastMagic1Time;
    private float lastMagic2Time;
    private float lastMagic3Time;

    private string currentStage;

    // 플레이어 층 구분용 Y 위치 범위
    public float floorYBottom = -3.5f;
    public float floorYMiddle = 0f;
    public float floorYTop = 3.5f;
    public float floorHeight = 1f;

    void Start()
    {
        currentStage = SceneManager.GetActiveScene().name;
    }

    void Update()
    {
        // Magic1: Z key
        if (Input.GetKeyDown(KeyCode.Z) && Time.time - lastMagic1Time >= magic1Cooldown)
        {
            FireMagic(magic1Prefab, 1);
            lastMagic1Time = Time.time;
            skillUI?.TriggerCooldown("Z");


            AudioManager.Instance?.PlaySkillSFX("Z");
        }

        // Magic2: X key
        if ((currentStage == "Stage2" || currentStage == "Stage3") &&
            Input.GetKeyDown(KeyCode.X) && Time.time - lastMagic2Time >= magic2Cooldown)
        {
            FireMagic(magic2Prefab, 2);
            lastMagic2Time = Time.time;
            skillUI?.TriggerCooldown("X");


            AudioManager.Instance?.PlaySkillSFX("X");
        }

        // Magic3: C key
        if (currentStage == "Stage3" &&
            Input.GetKeyDown(KeyCode.C) && Time.time - lastMagic3Time >= magic3Cooldown)
        {
            bool killed = KillClosestMonsterOnSameFloor();
            if (killed)
            {
                lastMagic3Time = Time.time;


                AudioManager.Instance?.PlaySkillSFX("C");
            }

            skillUI?.TriggerCooldown("C");
        }
    }

    // 마법 발사 함수에 데미지 전달
    void FireMagic(GameObject magicPrefab, int damage)
    {
        GameObject magic = Instantiate(magicPrefab, firePoint.position, Quaternion.identity);
        MagicProjectile mp = magic.GetComponent<MagicProjectile>();
        if (mp != null)
        {
            mp.damage = damage;
        }
        Destroy(magic, 2.0f);
    }

    // 마법3 기능
    bool KillClosestMonsterOnSameFloor()
    {
        float playerY = transform.position.y;
        float minY, maxY;

        if (Mathf.Abs(playerY - floorYBottom) < floorHeight / 2)
        {
            minY = floorYBottom - floorHeight / 2;
            maxY = floorYBottom + floorHeight / 2;
        }
        else if (Mathf.Abs(playerY - floorYMiddle) < floorHeight / 2)
        {
            minY = floorYMiddle - floorHeight / 2;
            maxY = floorYMiddle + floorHeight / 2;
        }
        else if (Mathf.Abs(playerY - floorYTop) < floorHeight / 2)
        {
            minY = floorYTop - floorHeight / 2;
            maxY = floorYTop + floorHeight / 2;
        }
        else
        {
            return false;
        }

        var monsters = MonsterManager.Instance?.monsters;
        if (monsters == null) return false;
        Monster closest = null;
        float closestDist = Mathf.Infinity;

        foreach (var m in monsters)
        {
            float my = m.transform.position.y;
            if (my >= minY && my <= maxY)
            {
                float dist = m.transform.position.x - transform.position.x;
                if (dist >= 0 && dist < closestDist)
                {
                    closest = m;
                    closestDist = dist;
                }
            }
        }

        if (closest != null)
        {
            Instantiate(magic3EffectPrefab, closest.transform.position, Quaternion.identity);

            Monster monsterScript = closest.GetComponent<Monster>();
            int scoreToAdd = 1;

            if (monsterScript != null)
            {
                scoreToAdd = monsterScript.scoreValue;
            }

            ScoreManager.Instance?.AddScore(scoreToAdd);

            Destroy(closest.gameObject);

            return true;
        }
        return false;
    }
}


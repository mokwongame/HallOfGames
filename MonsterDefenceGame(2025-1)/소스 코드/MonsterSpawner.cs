using UnityEngine;
using UnityEngine.SceneManagement;

public class MonsterSpawner : MonoBehaviour
{
    public GameObject monster1Prefab;
    public GameObject monster2Prefab;
    public GameObject monster3Prefab;

    public float spawnInterval = 2f;
    public Transform[] spawnPoints; // 몬스터가 나올 위치들 (층마다)

    private float timer = 0f;
    private string currentStage;

    void Start()
    {
        currentStage = SceneManager.GetActiveScene().name;
    }

    void Update()
    {
        timer += Time.deltaTime;

        if (timer >= spawnInterval)
        {
            SpawnMonster();
            timer = 0f;
        }
    }

    void SpawnMonster()
    {
        GameObject prefabToSpawn = GetRandomMonsterForStage();

        if (prefabToSpawn != null && spawnPoints.Length > 0)
        {
            int randIndex = Random.Range(0, spawnPoints.Length);
            Instantiate(prefabToSpawn, spawnPoints[randIndex].position, Quaternion.identity);
        }
    }

    GameObject GetRandomMonsterForStage()
    {
        if (currentStage == "Stage1")
        {
            return monster1Prefab;
        }
        else if (currentStage == "Stage2")
        {
            int choice = Random.Range(0, 2); // 0 or 1
            return choice == 0 ? monster1Prefab : monster2Prefab;
        }
        else if (currentStage == "Stage3")
        {
            int choice = Random.Range(0, 3); // 0 ~ 2
            if (choice == 0) return monster1Prefab;
            else if (choice == 1) return monster2Prefab;
            else return monster3Prefab;
        }

        return null;
    }
}

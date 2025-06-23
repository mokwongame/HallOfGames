using System.Collections.Generic;
using UnityEngine;

public class MonsterManager : MonoBehaviour
{
    public static MonsterManager Instance;

    public List<Monster> monsters = new List<Monster>();

    private void Awake()
    {
        if (Instance == null)
            Instance = this;
        else
            Destroy(gameObject);
    }

    public void Register(Monster monster)
    {
        if (!monsters.Contains(monster))
            monsters.Add(monster);
    }

    public void Unregister(Monster monster)
    {
        if (monsters.Contains(monster))
            monsters.Remove(monster);
    }
}

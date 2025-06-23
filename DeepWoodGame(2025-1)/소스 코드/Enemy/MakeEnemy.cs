using NUnit.Framework;
using System.Collections.Generic;
using UnityEngine;

public class MakeEnemy : MonoBehaviour
{
    [SerializeField] List<GameObject> Enemys = new List<GameObject> ();
    void Start()
    {
        InvokeRepeating("EnemyMake", 1, 1f);

    }

    private void EnemyMake()
    {
        int num = Random.Range(0, 3);
        if (num == 0)
        {
            GameObject temp = Instantiate(Enemys[Random.Range(0, Enemys.Count)]);
            temp.transform.position = new Vector2(transform.position.x, temp.transform.position.y);
        }
    }

    void Update()
    {

    }
}

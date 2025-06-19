using UnityEngine;

public class MakeMole : MonoBehaviour
{
    public GameObject mole;
    public GameObject moleking;
    public GameObject moleboom;
    public float startTime = 2.0f;
    public float repeatTime = 5.0f;
    public float xmin = -9.0f;
    public float xmax = 9.0f;
    public float zmin = -9.0f;
    public float zmax = 9.0f;

    GameObject[] molePrefabs;
    void Start()
    {
        InvokeRepeating("makeMole", startTime, repeatTime);
    }

    
    void Update()
    {

    }
    void makeMole()
    {
        Vector3 pos = transform.position;
        pos.x = Random.Range(xmin, xmax);
        pos.z = Random.Range(zmin, zmax);
        pos.y = -2.0f;
        float rand = Random.value;
        GameObject selectMole;

        if( rand < 0.6f)
        {
            selectMole = mole;
        }
        else if (rand < 0.85f)
        {
            selectMole = moleboom;
        }
        else
        {
            selectMole = moleking;
        }


            Instantiate(selectMole, pos, transform.rotation);
    }
}
